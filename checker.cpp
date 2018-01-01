#include<iostream>
#include<cstdlib>
#include<fstream>
#include <chrono>
using namespace std;

#ifdef __WIN32
    #define EXE(X) #X ".exe" 
#else
    #define EXE(X) "./" #X 
#endif

using chrono::duration;
#if defined(__GNUC__) && defined(__WIN32)
    using hclock = chrono::steady_clock;
#else
    using hclock = chrono::high_resolution_clock;
#endif
const char * AC = "Congratulations!! Your index.rpt passed the verification!!";
int main()
{
    string condir = "config/";
    string bendir = "bench/";
    string ben[] {  "DataReference_n_comp.lst",
                    "DataReference_n_real.lst",
                    "DataReference_real_up.lst",
                    "InstReference_iir_one.lst",
                    "InstReference_lms.lst",
                    "InstReference_matrix.lst",
                    "InstReference_n_comp.lst",
                    "randcase1.lst" };
    string con[] {  "cacheA.org",
                    "cacheB.org",
                    "cacheC.org",
                    "cacheD.org"    };
    cout<<"Rebuild program... " <<endl
        <<"  IF you did not know how to set g++ "<<endl
        <<"  just put executable file ( " EXE(arch_final) " )here!"<<endl;
    system("g++ -std=c++14 -O2 -Wall -Wextra arch_final.cpp -o " EXE(arch_final));
    string cmd;
    cout<<"start..."<<endl<<endl;
    for(auto c:con)
    {
        c = condir+c;
        for(auto b:ben)
        {
            b = bendir+b;
            cmd = EXE(arch_final) " "+c+" "+b;
            
            auto s = hclock::now();
            system(cmd.c_str());
            auto e = hclock::now();

            cmd = EXE(verify) " "+c+" "+b+" index.rpt > tmp.dat";
            system(cmd.c_str());
            
            ifstream fin("tmp.dat");
            getline(fin,cmd);
            if( cmd != AC )
            {
                cout<<EXE(arch_final) " "+c+" "+b<<endl;
                cout<<"========="<<endl;
                cout<<cmd<<endl;
                while( getline(fin,cmd) ) cout<<cmd<<endl;
                cout<<"========="<<endl<<endl;
                goto badend;
            }else{
                duration<double> diff = e - s;
                if( diff.count() > 60 )
                {
                    cout<<EXE(arch_final) " "+c+" "+b<<endl;
                    cout<<"TLE!!!"<<endl;
                    cout<<"  "<<diff.count()<<"s"<<endl;
                    cout<<"========="<<endl<<endl;
                    goto badend;
                }
            }
        }
    }
    cout<<AC<<endl;
    badend:;
    #ifdef __WIN32
    system("del tmp.dat");
    system("PAUSE");
    #else
    system("rm tmp.dat");
    #endif
}
