#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <cstddef>
#include <set>
#include <vector>

class Scheduler {
private:
   typedef std::vector<int> group_type;
   typedef std::vector< group_type > round_type;
   typedef std::vector< round_type > schedule_type;
   typedef std::set<std::pair<int, int> > pair_set;

public:
   Scheduler(size_t people, size_t groups);
   void solve() const;

private:
   // Helper functions
   void solve(schedule_type & schedule) const;
   void solve(schedule_type & schedule, round_type & round, std::vector<group_type>::const_iterator cur) const;

   bool valid_group(const group_type & group, const round_type & round) const;
   bool valid_group(const group_type & group, const schedule_type & schedule) const;

   bool contain_same_pair(const group_type & group1, const group_type & group2) const;

   void pairs(const group_type & group, pair_set & pairs) const;

   /// Calculate all the group combinations possible
   void group_combinations();
   void group_combinations(group_type & group, size_t cur, size_t depth);

   /// print the contents of the container
   void print(const group_type & group) const;
   void print(const round_type & round) const;
   void print(const schedule_type & schedule) const;

   size_t m_people;
   size_t m_groups;
   size_t m_rounds;
   std::vector<group_type> m_all_groups;
};

#endif
