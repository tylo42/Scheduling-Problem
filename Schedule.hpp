#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

#include <iostream>
#include <set>
#include <string>
#include <vector>

class Schedule {
public:
   typedef std::vector<int> group_type;
   typedef std::vector< group_type > round_type;

private:
   typedef std::vector< round_type > schedule_type;
   typedef std::set<std::pair<int, int> > pair_set;

public:
   Schedule(size_t people, size_t groups, size_t rounds);

   void push_round(const round_type & round);
   void pop_round();
   size_t round_size() const;
   bool valid_group(const group_type & group) const;

   friend std::ostream & operator<<(std::ostream & os, const Schedule & s);

private:
   /// print the contents of the container
   std::string to_string(const group_type & group) const;
   std::string to_string(const round_type & round) const;
   std::string to_string(const schedule_type & schedule) const;

   bool contain_same_pair(const group_type & group1, const group_type & group2) const;

   void pairs(const group_type & group, pair_set & pairs) const;

   size_t m_people;
   size_t m_groups;
   size_t m_rounds;
   schedule_type m_schedule;
};


std::ostream & operator<<(std::ostream & os, const Schedule & s);


#endif // SCHEDULE_HPP
