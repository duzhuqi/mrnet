/****************************************************************************
 * Copyright � 2003-2012 Dorian C. Arnold, Philip C. Roth, Barton P. Miller *
 *                  Detailed MRNet usage rights in "LICENSE" file.          *
 ****************************************************************************/

#if !defined(test_h)
#define test_h

#include "mrnet/MRNet.h"
#include "utils.h"

using namespace MRN;

#include <string>
#include <map>

#define char_t char
#define uchar_t unsigned char

namespace MRN_test {

extern std::map< MRN::DataType, std::string> Type2String;
extern std::map< MRN::DataType, const char *> Type2FormatString;

void val2string( char *, void *, MRN::DataType );
bool compare_Vals( void *, void *, MRN::DataType );

bool compare_Float(float f1, float f2, int sig);
bool compare_Double(double f1, double f2, int sig);

class Timer{
 public:
    struct timeval tv_start, tv_end;
    double d_start, d_end;

    void start(){
        while(gettimeofday(&tv_start, NULL) == -1) ;
        d_start = ( (double(tv_start.tv_sec))
                    + ((double)tv_start.tv_usec)/1000000.0 );
    }
    void end(){
        while(gettimeofday(&tv_end, NULL) == -1) ;
        d_end = ( (double(tv_end.tv_sec))
                    + ((double)tv_end.tv_usec)/1000000.0 );
    }
    double get_start( ){
        return d_start;
    }
    double get_end( ){
        return d_end;
    }
    double duration( ){
        return d_end - d_start;
    }
};

typedef enum {MRNTEST_NOTRUN, MRNTEST_FAILURE, MRNTEST_SUCCESS, MRNTEST_COMPLETED} TestStatus;

class Test{
 private:
    class SubTest{
    private:
        std::string name;
        FILE * fout;
        Timer timer;
        TestStatus status;
    public:
        SubTest(const std::string &subtest_name, FILE *f=stdout);
        void end(TestStatus s);
        void print_status();
    };

    std::string name;
    FILE * fout;
    int num_failures;
    Timer timer;
    std::map < std::string, SubTest* > subtests;

 public:
    Test(const char *test_name, FILE *f=stdout);
    ~Test();
    void end_Test( );
    int start_SubTest(const std::string & subtest_name);
    int end_SubTest(const std::string & subtest_name, TestStatus status);
    void print(const char *, const std::string& subtest_name="");
};

int ParseBECommandLine( int argc, char** argv,
                        std::string& hostname,
                        Rank& rank,
                        std::string& parent_hostname,
                        Rank& parent_rank,
                        Port& parent_port );

}

#endif /* test_h */
