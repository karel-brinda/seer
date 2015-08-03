/*
 * File: kmer.cpp
 *
 * Helper functions for the kmer class
 *
 */

#include "kmer.hpp"



// Initialisation
Kmer::Kmer(std::string sequence, std::vector<std::string> occurrences, double pvalue, double beta, long int position)
   :word(sequence), occurrences(occurrences), x_set(0), pvalue(pvalue), chi_pvalue(kmer_chi_pvalue_default), b(beta), position(position)
{
}

// Initialise without calculated information
Kmer::Kmer(std::string sequence, std::vector<std::string> occurrences)
   :word(sequence), occurrences(occurrences), x_set(0), pvalue(kmer_pvalue_default), chi_pvalue(kmer_chi_pvalue_default), b(kmer_beta_default), position(kmer_position_default)
{
}

// Initialise with default info only
Kmer::Kmer()
   :word(kmer_seq_default), occurrences(kmer_occ_default), x_set(0), pvalue(kmer_pvalue_default), chi_pvalue(kmer_chi_pvalue_default), b(kmer_beta_default), position(kmer_position_default)
{
}

// Map kmer to a reference file
long int Kmer::map(std::string& ref_file)
{
   throw std::logic_error("kmer position mapping not yet implemented");
}

// Output
std::ostream& operator<<(std::ostream &os, const Kmer& k)
{
   return os << k.sequence() << "\t" << std::scientific << k.chi_p_val() << "\t" << k.p_val() << "\t" << k.beta() << "\n";
}

// Input
std::istream& operator>>(std::istream &is, Kmer& k)
{
   std::vector<std::string> samples;
   std::string word, s_buffer, kmer_line;
   char c_buffer;

   /*
    * Example dsm file line AAAAAAAAAAAAAAAAAATGCATATTTATCTTAG 5.172314 0.175087 100 0 100
    * 0.164875 100 | 6925_3#7:9 6823_4#17:26 6871_2#9:8
    *
    * OR
    *
    * AAAAAAAAAAAAAAAAAATGCATATTTATCTTAG 5.172314 6925_3#7:9 6823_4#17:26 6871_2#9:8
    *
    */
   if (is)
   {
      std::getline(is, kmer_line);
      std::stringstream line(kmer_line);

      // First field is the kmer
      line >> word;

      // Skip entropy fields
      while (line >> s_buffer)
      {
         if (s_buffer == "|")
         {
            break;
         }
         // Gone one too far in a string w/out | separator
         else
         {
            size_t occurence_pos = s_buffer.find(":");
            if (s_buffer.find(":") != std::string::npos)
            {
               samples.push_back(s_buffer.substr(0, occurence_pos));
               break;
            }
         }
      }
      s_buffer = "";

      while (line >> c_buffer)
      {
         // Found end of sample. Read forward to next one
         if (c_buffer == ':')
         {
            samples.push_back(s_buffer);
            line >> s_buffer;

            s_buffer = "";
         }
         else
         {
            s_buffer += c_buffer;
         }
      }
   }

   // Keep samples sorted, for easy conversion into a vector
   std::sort(samples.begin(), samples.end());

   k = Kmer(word, samples);

   return is;
}
