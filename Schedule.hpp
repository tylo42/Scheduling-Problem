#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

#include <iostream>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

class Schedule {
public:
   typedef std::vector<int> group_type;
   typedef std::shared_ptr<group_type> group_ptr;
   typedef std::vector<group_ptr> round_type;

private:
   typedef std::vector<round_type> schedule_type;
   typedef std::pair<int, int> pair_type;
   typedef std::function<bool(const pair_type &, const pair_type &)> pair_less;
   typedef std::set<pair_type, pair_less> pair_set;
   typedef std::map<pair_type, size_t, pair_less> pair_usage_map;

public:
   Schedule(size_t people, size_t groups, size_t rounds);

   void push_round(const round_type & round);
   void pop_round();
   size_t round_size() const;
   bool valid_group(const group_type & group, size_t size) const;
   bool check(size_t min, size_t max) const;

   friend std::ostream & operator<<(std::ostream & os, const Schedule & s);

private:
   /// print the contents of the container
   std::string to_string(const group_type & group) const;
   std::string to_string(const round_type & round) const;
   std::string to_string(const schedule_type & schedule) const;

   bool consistent() const;

   bool contain_same_pair(const group_type & group1, const group_type & group2) const;

   void pairs(const group_type & group, pair_set & pairs) const;
   void matching_pairs(const group_type & group1, const group_type & group2, pair_set & result) const;

   size_t m_people;
   size_t m_groups;
   size_t m_rounds;
   schedule_type m_schedule;
};


std::ostream & operator<<(std::ostream & os, const Schedule & s);


#endif // SCHEDULE_HPP
