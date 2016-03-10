/*
 *
 * seercommon.hpp
 * Header file for seercommon
 * Shared functions between seer and kmds
 *
 */

// C/C++/C++11 headers
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <chrono>
#include <iterator>
#include <vector>
#include <unordered_map>
#include <thread>
#include <exception>
#include <sys/stat.h>
#include <regex>

// gzstream headers
#include <gzstream.h>

// Boost headers
#include <boost/program_options.hpp>
#include <boost/math/distributions/normal.hpp>
#include <boost/math/distributions/students_t.hpp>

// Armadillo/dlib headers
#define ARMA_DONT_PRINT_ERRORS
#include <armadillo>
#include <dlib/matrix.h>

// Classes
#include "kmer.hpp"
#include "sample.hpp"
#include "covar.hpp"

// Constants
//    Default options
const double maf_default = 0.01;
const long int max_length_default = 100;
const std::string chisq_default = "10e-5";

typedef dlib::matrix<double,0,1> column_vector;

const std::string sample_suffix = ".samples";

// Structs
struct cmdOptions
{
   double log_cutoff;
   double chi_cutoff;

   long int max_length;
   long int size;
   int filter;
   int positive;
   int pc;
   int print_samples;
   int write_distances;
   unsigned int num_threads;
   size_t min_words;
   size_t max_words;

   std::string pheno;
   std::string kmers;
   std::string output;
};

// Function headers
//    seerCommon.cpp
cmdOptions verifyCommandLine(boost::program_options::variables_map& vm, const std::vector<Sample>& samples);
void verifyMDSOptions(cmdOptions& verified, boost::program_options::variables_map& vm);

arma::vec dlib_to_arma(const column_vector& dlib_vec);
column_vector arma_to_dlib(const arma::vec& arma_vec);
arma::mat vecToMat(const std::vector<std::string>& in_col);
void normaliseMatCols(arma::mat& matrix_in);

int continuousPhenotype (const std::vector<Sample>& sample_list);

arma::mat inv_covar(arma::mat A);

// seerErr headers
void badCommand(const std::string& command, const std::string& value);

// seerIO headers
void readPheno(const std::string& filename, std::vector<Sample>& samples, std::unordered_map<std::string,int>& sample_map);
void openDsmFile(igzstream& dsm_file, const std::string& file_name);

arma::vec constructVecY(const std::vector<Sample>& samples);
arma::vec constructVecX(const Kmer& k, const std::vector<Sample>& samples);

arma::mat readHDF5(const std::string& file_name);

void writeMDS(const std::string& file_name, const std::vector<Sample>& sample_names, const arma::mat& MDS);
void writeDistances(const std::string& file_name, const arma::mat& distances);
arma::mat readMDS(const std::string& file_name, const std::vector<Sample>& sample_names);
arma::mat readMDSList(const std::string& filename);

arma::mat parseCovars(const std::string& file, const std::string& columns);
std::vector<std::tuple<int,bool>> parseCovarColumns(const std::string& columns);
arma::mat encodeDummy(const std::vector<std::string>& in_col);

int fileStat(const std::string& filename);

// seerFilter headers
int passFilters(const cmdOptions& filterOptions, Kmer& k, const std::vector<Sample>& samples, const arma::vec& y, const int continuous_phenotype, const bool do_stats = 1);
int passBasicFilters(const Kmer& k, const int max_length, const int min_words, const int max_words);
int passStatsFilters(Kmer& k, const arma::vec& y, const double chi_cutoff, const int continuous_phenotype, const int positive_only);

// seerChiFilter headers
double chiTest(Kmer& k, const arma::vec& y);
double welchTwoSamplet(const Kmer& k, const arma::vec& y);
double normalPval(double testStatistic);

