/*
 **************************************************************************
 *                                                                        *
 *                        Open Bloom Filter Test                          *
 *                                                                        *
 * Author: Arash Partow - 2000                                            *
 * URL: http://www.partow.net                                             *
 * URL: http://www.partow.net/programming/hashfunctions/index.html        *
 *                                                                        *
 * Copyright notice:                                                      *
 * Free use of the General Purpose Hash Function Algorithms Library is    *
 * permitted under the guidelines and in accordance with the most current *
 * version of the Common Public License.                                  *
 * http://www.opensource.org/licenses/cpl.php                             *
 *                                                                        *
 **************************************************************************
*/

#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <string>

#include "bloom_filter.h"


void read_file(const std::string file_name, std::vector<std::string>& buffer);
std::string reverse(std::string str);

int main(int argc, char* argv[])
{

   std::vector<std::string> word_list;

   read_file("word-list.txt",word_list);

   if (word_list.empty())
   {
      std::cout << "ERROR: Input file invalid!" << std::endl;
      return false;
   }

   bloom_filter  filter1(word_list.size(),1.0/(5.0 * word_list.size()),0x00000000);
   bloom_filter  filter2(word_list.size(),1.0/(5.0 * word_list.size()),0x00000000);
   bloom_filter  filter = filter1 & filter2;  // intersection of filter1 and filter2

   for(std::size_t i = 0; i < word_list.size(); i++)
   {
      filter.insert(word_list[i]);
   }

   for(std::size_t i = 0; i < word_list.size(); i++)
   {
      if (!filter.contains(word_list[i]))
      {
         std::cout << "ERROR: key not found!" << std::endl;
      }
   }

   for(std::size_t i = 0; i < word_list.size(); i++)
   {
      if (filter.contains(word_list[i] + reverse(word_list[i])))
      {
         std::cout << "ERROR: key that does not exist found! => " << (word_list[i] + reverse(word_list[i])) << std::endl;
      }

      if (filter.contains(word_list[i] + word_list[i]))
      {
         std::cout << "ERROR: key that does not exist found! => " << (word_list[i] + reverse(word_list[i])) << std::endl;
      }

      if (filter.contains(reverse(word_list[i]) + word_list[i] + reverse(word_list[i])))
      {
         std::cout << "ERROR: key that does not exist found! => " << reverse(word_list[i]) + word_list[i] + reverse(word_list[i]) << std::endl;
      }

      if (filter.contains(word_list[i] + reverse(word_list[i]) + word_list[i]))
      {
         std::cout << "ERROR: key that does not exist found! => " << word_list[i] + reverse(word_list[i]) + word_list[i] << std::endl;
      }
   }

   return true;
}


void read_file(const std::string file_name, std::vector<std::string>& buffer)
{
   /* Assumes no whitespace in the lines being read in. */
   std::ifstream in_file(file_name.c_str());
   if (!in_file)
   {
      return;
   }

   std::istream_iterator< std::string > is(in_file);
   std::istream_iterator< std::string > eof;
   std::copy( is, eof, std::back_inserter(buffer));
}

std::string reverse(std::string str)
{
   char tempch;

   /* Reverse the string */
   for(unsigned int i = 0; i < (str.length() / 2); i++)
   {
      tempch = str[i];
      str[i] = str[str.length() - i - 1];
      str[str.length() - i - 1] = tempch;
   }

   return str;
}
