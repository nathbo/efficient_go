#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <set>

using namespace std;

int main(int argc,char* argv[])
{
    if (argc!=4) {
        cerr << "usage: " << argv[0] << " lineitem.tbl orders.tbl customer.tbl" << endl;
        return 1;
    }
    
    // Process input segment parameters
    vector<string> PARAMS;
    string SEGMENT_PARAM;
    while (cin >> SEGMENT_PARAM){
        PARAMS.push_back(SEGMENT_PARAM);
    }
    
    // Open all files and figre out all lengths
    // 1
    int l_handle=open(argv[1],O_RDONLY);
    if (l_handle<0) return 1;
    lseek(l_handle,0,SEEK_END);
    long l_length=lseek(l_handle,0,SEEK_CUR);
    auto l_data=static_cast<const char*>(mmap(nullptr,l_length,PROT_READ,MAP_SHARED,l_handle,0));
    auto l_dataLimit=l_data+l_length;
    auto l_safeLimit=(l_length>8)?l_dataLimit-8:l_data;

    // 2
    int o_handle=open(argv[2],O_RDONLY);
    if (o_handle<0) return 1;
    lseek(o_handle,0,SEEK_END);
    long o_length=lseek(o_handle,0,SEEK_CUR);
    auto o_data=static_cast<const char*>(mmap(nullptr,o_length,PROT_READ,MAP_SHARED,o_handle,0));
    auto o_dataLimit=o_data+o_length;
    auto o_safeLimit=(o_length>8)?o_dataLimit-8:o_data;

    // 3
    int c_handle=open(argv[3],O_RDONLY);
    if (c_handle<0) return 1;
    lseek(c_handle,0,SEEK_END);
    long c_length=lseek(c_handle,0,SEEK_CUR);
    auto c_data=static_cast<const char*>(mmap(nullptr,c_length,PROT_READ,MAP_SHARED,c_handle,0));
    auto c_dataLimit=c_data+c_length;
    auto c_safeLimit=(c_length>8)?c_dataLimit-8:c_data;

    
    // First get all custkeys where c_mktsegment=<SEGMENT_PARAM>
    vector<set<int>> custkeys(PARAMS.size());
    const char* line=c_data;
    for (const char* current=c_data; current!=c_dataLimit;) {
        const char* last=line;
        unsigned column=0;
        unsigned int id = 0;
        for (;current!=c_dataLimit;++current) {
            char c=*current;
            if (column==0 && c!='|'){
                // cout << "id before: " << id << "current char: " << c << endl;
                id = 10*id + c - '0';
            }
            if (c=='|') {
                if (column==6) {
                    string segment="";
                    for (const char* iter=last,*limit=current;iter!=limit;++iter) {
                        c = *iter;
                        // cout << "Segment so far: " << segment << endl;
                        // cout << "iter so far: " << c << endl;
                        segment += c;
                    }
                    for (int i=0; i<PARAMS.size(); i++){
                        if (segment == PARAMS[i]){
                            // cout << id << endl;
                            custkeys[i].insert(id);
                        }
                    }
                    ++current;
                    while (current<c_safeLimit) {
                        uint64_t block=*reinterpret_cast<const uint64_t*>(current);
                        constexpr uint64_t highBits=0x8080808080808080ull;
                        constexpr uint64_t lowBits=~highBits;
                        constexpr uint64_t pattern=0x0A0A0A0A0A0A0A0Aull;
                        uint64_t lowChars=(~block)&highBits;
                        uint64_t found0A=~((((block&lowBits)^pattern)+lowBits)&highBits);
                        uint64_t match=found0A&lowChars;
                        if (!match) {
                            current+=8;
                            continue;
                        }
                        match=__builtin_bswap64(match);
                        current+=(__builtin_clzll(match)/8);
                        goto c_found0A;
                    }
                    for (;current!=c_dataLimit;++current)
                        if ((*current)=='\n')
                            break;
                    c_found0A:
                        
                    if (current!=c_dataLimit)
                        ++current;
                    break;
                } else {
                    last=current+1;
                    ++column;
                }
            } else if (c=='\n') {
                ++current;
                break;
            }
        }
    }

    // for (int i=0; i<PARAMS.size(); i++){
    //     cout << "Total amount of custkeys for given parameter (" << PARAMS[i] << "): " << custkeys[i].size() << endl;
    // }


    // Next get all orderkeys where custkey apperears in our custkeys
    vector<set<int>> orderkeys(PARAMS.size());

    line=o_data;
    for (const char* current=o_data; current!=o_dataLimit;) {
        const char* last=line;
        unsigned column=0;
        unsigned int id = 0;
        for (;current!=o_dataLimit;++current) {
            char c=*current;
            if (column==0 && c!='|'){
                id = 10*id + c - '0';
            }
            if (c=='|') {
                if (column==1) {
                    unsigned int ckey=0;
                    for (const char* iter=last,*limit=current;iter!=limit;++iter) {
                        unsigned c=(*iter)-'0';
                        if (c>9) {
                            cerr << "invalid number in " << line << endl;
                            return 1;
                        }
                        ckey=10*ckey+c;
                    }
                    for (int i=0; i<PARAMS.size(); i++){
                        if(custkeys[i].count(ckey)) {
                            // cout << id << endl;
                            orderkeys[i].insert(id);
                        }
                    }
                    ++current;
                    while (current<o_safeLimit) {
                        uint64_t block=*reinterpret_cast<const uint64_t*>(current);
                        constexpr uint64_t highBits=0x8080808080808080ull;
                        constexpr uint64_t lowBits=~highBits;
                        constexpr uint64_t pattern=0x0A0A0A0A0A0A0A0Aull;
                        uint64_t lowChars=(~block)&highBits;
                        uint64_t found0A=~((((block&lowBits)^pattern)+lowBits)&highBits);
                        uint64_t match=found0A&lowChars;
                        if (!match) {
                            current+=8;
                            continue;
                        }
                        match=__builtin_bswap64(match);
                        current+=(__builtin_clzll(match)/8);
                        goto o_found0A;
                    }
                    for (;current!=o_dataLimit;++current)
                        if ((*current)=='\n')
                            break;
                    o_found0A:
                        
                    if (current!=o_dataLimit)
                        ++current;
                    break;
                } else {
                    last=current+1;
                    ++column;
                }
            } else if (c=='\n') {
                ++current;
                break;
            }
        }
    }
    
    // for (int i=0; i<PARAMS.size(); i++){
    //     cout << "Total amount of orderkeys for given parameter (" << PARAMS[i] << "): " << orderkeys[i].size() << endl;
    // }


    // Next sum up the quantities while counting how many of them were summed to get the avg
    vector<int> total_quantity(PARAMS.size(), 0);
    vector<int> counter(PARAMS.size(), 0);

    line=l_data;
    for (const char* current=l_data; current!=l_dataLimit;) {
        const char* last=line;
        unsigned column=0;
        unsigned int okey = 0;
        for (;current!=l_dataLimit;++current) {
            char c=*current;
            if (column==0 && c!='|'){
                okey = 10*okey + c - '0';
            }
            if (c=='|') {
                if (column==4) {
                    unsigned quantity=0;
                    for (const char* iter=last,*limit=current;iter!=limit;++iter) {
                        unsigned c=(*iter)-'0';
                        if (c>9) {
                            cerr << "invalid number in " << line << endl;
                            return 1;
                        }
                        quantity=10*quantity+c;
                    }
                    for (int i=0; i<PARAMS.size(); i++){
                        if(orderkeys[i].count(okey)) {
                            total_quantity[i] += quantity;
                            counter[i]++;
                        }
                    }
                    ++current;
                    while (current<l_safeLimit) {
                        uint64_t block=*reinterpret_cast<const uint64_t*>(current);
                        constexpr uint64_t highBits=0x8080808080808080ull;
                        constexpr uint64_t lowBits=~highBits;
                        constexpr uint64_t pattern=0x0A0A0A0A0A0A0A0Aull;
                        uint64_t lowChars=(~block)&highBits;
                        uint64_t found0A=~((((block&lowBits)^pattern)+lowBits)&highBits);
                        uint64_t match=found0A&lowChars;
                        if (!match) {
                            current+=8;
                            continue;
                        }
                        match=__builtin_bswap64(match);
                        current+=(__builtin_clzll(match)/8);
                        goto l_found0A;
                    }
                    for (;current!=l_dataLimit;++current)
                        if ((*current)=='\n')
                            break;
                    l_found0A:
                        
                    if (current!=l_dataLimit)
                        ++current;
                    break;
                } else {
                    last=current+1;
                    ++column;
                }
            } else if (c=='\n') {
                ++current;
                break;
            }
        }
    }

    vector<double> avg;
    for (int i=0; i<PARAMS.size(); i++){
        avg.push_back(double(total_quantity[i]) / counter[i]);
        // cout << "Computed average lineitem quantity for given parameter (" << PARAMS[i] << "): " << avg[i] << endl;
        cout << avg[i] << endl;
    }
}