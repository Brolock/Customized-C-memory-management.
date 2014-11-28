#ifndef TREE_H_
# define TREE_H_

# include <tuple>
# include <memory>

# include <iostream>
# include <algorithm>
# include <iterator>

namespace slwn
{
    /* operator+ not defined for tuple<UnknownTypes...> */
    template<class... Types>
    std::tuple<Types...>&
    operator+(std::tuple<Types...>&, std::tuple<Types...>&) = delete;

    /* operator+ defined for tuple<int, int> for readability purpose */
    inline
    std::tuple<int, int>
    operator+(const std::tuple<int, int>&& lhs,
              const std::tuple<int, int>&& rhs)
    {
        std::tuple<int, int> result;

        std::get<0>(result) = std::get<0>(lhs) + std::get<0>(rhs);
        std::get<1>(result) = std::get<1>(lhs) + std::get<1>(rhs);

        return result;
    }
}

namespace slwn
{
    /* The recursive aspect of every function in BaseTree is maybe to change */
    /* TODO measurement */
    template<class... Types>
    class BaseTree
    {
        /* The sons of a node are sons_ plus the sons_->brothers_ */
    protected:
        BaseTree *sons_ = nullptr;
        BaseTree *brothers_ = nullptr;

    public: 
        /* print the infos of the Tree */
        virtual void
        print(std::ostream&, size_t) const = 0;

        /* add a son to the current node */
        void
        add_son(BaseTree *son)
        {
            if (!son_already_exists(son))
            {
                if (sons_ != nullptr)
                    sons_->add_brother(son);
                else
                    sons_ = son;
            }
        }
    private:
        bool son_already_exists(BaseTree *son)
        {
            BaseTree *son_it = sons_;
            while (son_it != nullptr)
            {
                if (son_it == son)
                    return true;
                else
                    son_it = son_it->brothers_;
            }
            return false;
        }

    protected:
        /* tuple infos of the current node plus all it's sons (recusively) */
        std::tuple<Types...>
        get_branch_infos() const
        {
            return get_node_infos() + get_sons_infos();
        }
    
    private:
        /* add a brother to the node (is called if sons_ already exist) */
        void
        add_brother(BaseTree *brother)
        {
            if (brothers_ != nullptr)
                brothers_->add_brother(brother);
            else
                brothers_ = brother;
        }

        /** Tuple infos getter functions **/ 
    private:
        /* return the tuple infos of the current node */
        virtual std::tuple<Types...>
        get_node_infos() const = 0;

        /* tuple infos of the current node and all its brothers and sons */
        std::tuple<Types...>
        get_all_infos() const
        {
            return get_node_infos() + get_sons_infos() + get_brothers_infos();
        }

        /* tuple infos of the current node sons */
        std::tuple<Types...>
        get_sons_infos() const
        {
            if (sons_ != nullptr)
                return sons_->get_all_infos();
            else
                return std::tuple<Types...>();
        }
    
        /* tuple infos of the current node brothers and there sons */
        std::tuple<Types...>
        get_brothers_infos() const
        {
            if (brothers_ != nullptr)
                return brothers_->get_all_infos();
            else
                return std::tuple<Types...>();
        }
    };
}

#endif // !TREE_H_
