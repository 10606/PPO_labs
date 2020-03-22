#ifndef PROCESSING_H
#define PROCESSING_H

#include "../utils/time_convert.h"
#include "../dao/postgres.h"

template <typename DB = DAO, typename TIME = timer>
struct manager_t
{
    manager_t (std::shared_ptr <DB> _dao) :
        dao(_dao)
    {}

    void create_abonement (abonement const & abonement)
    {
        dao->create_abonement(abonement);
    }

    void continue_abonement (abonement_continue const & abonement_con)
    {
        dao->continue_abonement(abonement_con);
    }


    static abonement get_abonement_info (int id, std::shared_ptr <DB> dao)
    {
        abonement ans = dao->get_abonement(id);
        std::vector <abonement_continue> continues = dao->get_abonement_continues(id);
        if (!continues.empty())
        {
            ans.use_to = continues.back().use_to;
        }
        return ans;
    }
    
    abonement get_abonement_info (int id)
    {
        return get_abonement_info(id, dao);
    }

private:
    std::shared_ptr <DB> dao;
};

template <typename DB = DAO, typename TIME = timer>
struct enter_t
{
    enter_t (std::shared_ptr <DB> _dao) :
        dao(_dao),
        time()
    {}
    
    void entering (int id)
    {
        abonement abon = manager_t <DB, TIME> ::get_abonement_info(id, dao);
        time_t tm = get_time(abon.use_to);
    
        std::time_t t = time.get_time();
        if (tm < t)
        {
            //std::cout << abon.use_to << " " << t << " " <<  tm << "\n";
            throw std::runtime_error("abonement need continue (" + abon.use_to + ")");
        }
        
        dao->entering(enter(id, to_string(t)));
    }

private:
    std::shared_ptr <DB> dao;
    TIME time;
};

template <typename DB = DAO, typename TIME = timer>
struct statistic_t
{
    struct day_cnt
    {
        day_cnt (std::tm const & t, std::time_t _g_time, double _cnt = 0.5) :
            tm_day(t.tm_mday),
            tm_mon(t.tm_mon),
            tm_year(t.tm_year),
            g_time(_g_time),
            cnt(_cnt)
        {}
        
        int tm_day;
        int tm_mon;
        int tm_year;
        std::time_t g_time;
        double cnt;
    };

    struct intervals
    {
        intervals () :
            sum_time(0),
            cnt(0)
        {}
        
        double sum_time;
        double cnt;
    };
    
    
    statistic_t (std::shared_ptr <DB> _dao) :
        duration(),
        frequency(),
        dao(_dao),
        inside(),
        by_day()
    {
        std::vector <enter> enters = dao->get_all_enter_events();
        //std::cout << "enters " << enters.size() << "\n";
        for (enter const & e : enters)
        {
            event(e);
        }

        dao->subscribe
        (
            [this] (enter const & e) -> void 
            {
                event(e);
            }
        );
    }

    std::string get_stat ()
    {
        std::string answer;
        double duration_ =  duration.sum_time  / duration.cnt;
        double frequency_ = frequency.sum_time / frequency.cnt;

        answer += "duration  = " + std::to_string(duration_) + "\n";
        answer += "frequency = " + std::to_string(frequency_) + "\n";
        for (auto i : by_day)
        {
            answer += (to_string(i.g_time) + " " + std::to_string(i.cnt)) + "\n";
        }
        return answer;
    }
    
    void event (enter const & e)
    {
        std::tm loc_time = get_local_time(e.time);
        std::time_t g_time = std::mktime(&loc_time);
        
        std::map <int, std::time_t> :: iterator it;
        if ((it = inside.find(e.id)) != inside.end())
        {
            duration.sum_time += static_cast <double> (g_time - it->second) / 60.;
            //std::cout << g_time << " " << it->second << "\n";
            duration.cnt++;
            inside.erase(it);
        }
        else
        {
            frequency.sum_time++;
            if (by_day.empty() ||
                (
                    (loc_time.tm_mday != by_day.back().tm_day) || 
                    (loc_time.tm_mon  != by_day.back().tm_mon) || 
                    (loc_time.tm_year != by_day.back().tm_year)
                ))
            {
                by_day.push_back(day_cnt(loc_time, g_time, 1));
                frequency.cnt++;
            }
            else 
            {
                by_day.back().cnt += 1;
            }
            inside.insert(it, {e.id, g_time});
        }

        //std::cout << "inside " << inside.size() << "\n";
    }

private:
    
    intervals duration;
    intervals frequency;
    std::shared_ptr <DB> dao;
    std::map <int, time_t> inside;
    std::vector <day_cnt> by_day; //day -> cnt
};

#endif
