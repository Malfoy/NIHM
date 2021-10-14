#include "nihm_index.h"
#include "optionparser.h"

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <atomic>
#include <mutex>
#include <stdint.h>
#include <unordered_map>
#include <pthread.h>
#include <chrono>
#include <omp.h>
#include <math.h>
#include <unistd.h>
#include <bitset>



using namespace std;
using namespace chrono;

struct Arg: public option::Arg {
  static void printError(const char* msg1, const option::Option& opt, const char* msg2) {
    fprintf(stderr, "%s", msg1);
    fwrite(opt.name, opt.namelen, 1, stderr);
    fprintf(stderr, "%s", msg2);
  }
  static option::ArgStatus Unknown(const option::Option& option, bool msg) {
    if (msg) printError("Unknown option '", option, "'\n");
    return option::ARG_ILLEGAL;
  }
  static option::ArgStatus NonEmpty(const option::Option& option, bool msg) {
    if (option.arg != 0 && option.arg[0] != 0)
      return option::ARG_OK;
    if (msg) printError("Option '", option, "' requires a non-empty argument\n");
    return option::ARG_ILLEGAL;
  }
  static option::ArgStatus Numeric(const option::Option& option, bool msg) {
    char* endptr = 0;
    if (option.arg != 0 && strtol(option.arg, &endptr, 10)){};
    if (endptr != option.arg && *endptr == 0)
      return option::ARG_OK;
    if (msg) printError("Option '", option, "' requires a numeric argument\n");
    return option::ARG_ILLEGAL;
  }
};


enum  optionIndex {
  UNKNOWN,
  LIST,
  KMER,
  QUERY_FILE,
  OUTPUT,
  LOGO_OPT,
  HELP,
};



const option::Descriptor usage[] = {
  {UNKNOWN, 0,"" , ""    , Arg::Unknown,
    "\n"
      "Usage: " PACKAGE " [options]\n\n"
      "* Where options can be:" },
  {LIST, 0, "l" , "list" ,Arg::NonEmpty,
    "  --list, -l <file> "
      "\tUse the content of the given (raw formatted) <file> to load genomes.\v"
      "Example:"
      "\v  --list my_genomes.lst" },
  {QUERY_FILE, 0, "F", "query-file"    , Arg::NonEmpty,
    "  --query-file, -F <sequence_files> "
      "\tFor each sequence in the <sequence_files> search the sequence in the index and print the genomes"
      " with this sequence.\v"
      "The query file can be either a fasta formatted file (each sequence being a query) or a one line "
      "raw query formatted file (following the same syntax as the query '--option'). In the latter case, "
      "the only requirement is that the first character of the query file is different from '>' ; One "
      "can also add comments starting with '#' until the end of line.\v"
      "Examples:"
      "\v --query-file 'sequence_files'" },
  {OUTPUT, 0, "o", "output", Arg::NonEmpty,
    "  --output, -o <filename> "
      "\tDump the current index in text format to the given file."
  },
  {LOGO_OPT, 0, "",  "logo", Arg::None,
    "  --logo "
      "\tPrint ASCII art logo, then exit."
  },
  {UNKNOWN, 0,"" , ""    , Arg::Unknown, "\n* Other usage:"},
  {HELP,  0, "h" , "help"  ,Arg::None,
    "  --help, -h  "
      "\tPrint usage and exit." }
};

option::Option *options = NULL, *buffer = NULL;
void deleteOptsArrays() {
  if (options) {
    delete [] options;
  }
  if (buffer) {
    delete [] buffer;
  }
}



int main(int argc, char ** argv){
  string filename = "";
  argc-=(argc>0); argv+=(argc>0); // skip program name argv[0] if present
  option::Stats  stats(true, usage, argc, argv);
  options = new option::Option[stats.options_max];
  buffer = new option::Option[stats.buffer_max];
  atexit(deleteOptsArrays);
  option::Parser parse(usage, argc, argv, options, buffer);

  if (parse.error()) {
    cout << "Bad usage!!!" << endl;
    return EXIT_FAILURE;
  }
}


/**********************************/
/* Check Help and Version options */
/**********************************/
if (options[HELP] || argc == 0) {
  return EXIT_SUCCESS;
}


/************************************/
/* Complain about unknown arguments */
/************************************/
for (int i = 0; i < parse.nonOptionsCount(); ++i) {
  DEBUG_MSG("Non-option argument #" << i << " is " << parse.nonOption(i));
  log_error << "Ignoring unknown argument '" << parse.nonOption(i) << "'" << endl;
}
if (parse.nonOptionsCount()) {
  log_error << endlog;
  return EXIT_FAILURE;
}

Index monidex(16,31,10,4);
time_point<system_clock> start, endindex,end;

start = std::chrono::system_clock::now();

/*****************************************/
/* Add the genomes given in config files */
/*****************************************/
for (option::Option* opt = options[LIST];
    cout << "TODO ADD GENOOMES " << endl;

    monidex.insert_file_of_file_whole("/home/bilbok/Documents/NiHM/Tools/dashing/fof.txt");
    endindex = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endindex - start;

    std::cout << "Indexing lasted " << elapsed_seconds.count() << "s\n";
    monidex.query_file_of_file_whole("/home/bilbok/Documents/NiHM/Tools/dashing/fof.txt");
    end = std::chrono::system_clock::now();
    elapsed_seconds =end-  endindex;
    cout << "Query lasted " << elapsed_seconds.count() << "s\n";
    elapsed_seconds =end-  start;
    cout<<"whole run tool took " << elapsed_seconds.count() << endl;



    /**********************************************/
    /* Display the ASCII art logo of the program. */
    /**********************************************/
    if (options[LOGO_OPT]) {
      ifstream logo ("../resources/redoak.ascii");
      string line;

      if (logo.is_open()){
        while ( getline (logo,line) ){
          cout << line << '\n';
        }
        logo.close();
      }

      else cout << "Unable to open file";
      return EXIT_SUCCESS;
    }


return 0;
}
