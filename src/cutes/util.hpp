#ifndef _CUTES_UTIL_HPP_
#define _CUTES_UTIL_HPP_

template <typename Enter, typename Exit>
struct Scope
{
    Scope(Enter enter_fn, Exit exit_fn)
        : finalize_(true)
        , exit_fn_(exit_fn)
    {
        enter_fn();
    }

    Scope(Scope &&src)
        : finalize_(true), exit_fn_(src.exit_fn_)
    {
        src.finalize_ = false;
    }
    ~Scope()
    {
        if (finalize_) exit_fn_();
    }
    bool finalize_;
    Exit exit_fn_;
private:
    Scope(Scope&);
};

template <typename Enter, typename Exit>
Scope<Enter, Exit> mk_scope(Enter enter_fn, Exit exit_fn)
{
    return Scope<Enter, Exit>(enter_fn, exit_fn);
}

template <typename ContainerT, typename FnT>
ContainerT filter(ContainerT const& src, FnT fn) {
    ContainerT res;
    for (auto &v: src) {
        if (fn(v))
            res.append(v);
    }
    return res;
}

#define STRINGIFY(x) #x
#define DQUOTESTR(x) STRINGIFY(x)

#endif // _CUTES_UTIL_HPP_
