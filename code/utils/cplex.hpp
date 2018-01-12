#ifndef CF_SOLVE_CPLEX_HPP
#define CF_SOLVE_CPLEX_HPP

/* needed for some MAC versions of CPLEX */
#include <cstring>
#include <sstream>
#include <chrono>
#include <vector>

#define IL_STD

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wignored-attributes"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4512)
#endif

#include <ilcplex/ilocplex.h>
#include <ilcplex/ilocplexi.h>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

#ifndef CPLEX_HPP_NO_BOOST

#include <boost/config.hpp>

#define CPLEX_HPP_NORETURN BOOST_NORETURN
#else
#define CPLEX_HPP_NORETURN
#endif

namespace cpxhelper
{
  class user_cut_callback : public IloCplex::UserCutCallbackI
  {
  public:
    using IloCplex::UserCutCallbackI::add;
    using IloCplex::UserCutCallbackI::addLocal;
    using IloCplex::UserCutCallbackI::getValue;

  protected:
    user_cut_callback(IloEnv env) : IloCplex::UserCutCallbackI(env)
    {}

    ~user_cut_callback()
    {}
  };

  class lazy_constraint_callback : public IloCplex::LazyConstraintCallbackI
  {
  public:
    using IloCplex::LazyConstraintCallbackI::add;
    using IloCplex::LazyConstraintCallbackI::getValue;

  protected:
    lazy_constraint_callback(IloEnv env) : IloCplex::LazyConstraintCallbackI(env)
    {}

    ~lazy_constraint_callback()
    {}
  };

  class branch_callback : public IloCplex::BranchCallbackI
  {
  public:
    using IloCplex::BranchCallbackI::getEnv;
    using IloCplex::BranchCallbackI::makeBranch;
    using IloCplex::BranchCallbackI::prune;
    using IloCplex::BranchCallbackI::isIntegerFeasible;
    using IloCplex::BranchCallbackI::getBranchType;
    using IloCplex::BranchCallbackI::getNbranches;
    using IloCplex::BranchCallbackI::getBranch;
    using IloCplex::BranchCallbackI::getNodeData;
    using IloCplex::BranchCallbackI::getValue;
    using IloCplex::BranchCallbackI::getValues;

  protected:
    branch_callback(IloEnv env) : IloCplex::BranchCallbackI(env)
    {}

    ~branch_callback()
    {}
  };

  class node_callback : public IloCplex::NodeCallbackI
  {
  protected:
    node_callback(IloEnv env) : IloCplex::NodeCallbackI(env)
    {}

    ~node_callback()
    {}
  };

  class incumbent_callback : public IloCplex::IncumbentCallbackI
  {
  public:
    using IloCplex::IncumbentCallbackI::reject;
    using IloCplex::IncumbentCallbackI::getValue;
    using IloCplex::IncumbentCallbackI::getObjValue;
    using IloCplex::IncumbentCallbackI::getNodeData;

  protected:
    incumbent_callback(IloEnv env) : IloCplex::IncumbentCallbackI(env)
    {}

    ~incumbent_callback()
    {}
  };

  template<typename CallingType>
  class callback_container
  {
  public:
    callback_container(IloEnv env, CallingType &calling, bool time_cuts = false, bool time_lazy = false,
                       bool time_branch = false, bool time_node = false, bool time_incumbent = false) :
            M_calling(calling),
            M_cut(new(env) cut_cb(env, *this)),
            M_lazy(new(env) lazy_cb(env, *this)),
            M_branch(new(env) branch_cb(env, *this)),
            M_node(new(env) node_cb(env, *this)),
            M_inc(new(env) inc_cb(env, *this)),
            M_branch_cb(nullptr),
            M_node_cb(nullptr),
            M_inc_cb(nullptr),
            M_time_cuts(time_cuts),
            M_time_lazy(time_lazy),
            M_time_branch(time_branch),
            M_time_node(time_node),
            M_time_inc(time_incumbent),
            M_cut_time(0),
            M_lazy_time(0),
            M_branch_time(0),
            M_node_time(0),
            M_inc_time(0),
            M_cut_calls(0),
            M_lazy_calls(0),
            M_branch_calls(0),
            M_node_calls(0),
            M_inc_calls(0)
    {}

    ~callback_container()
    {
      M_cut.end();
      M_lazy.end();
      M_branch.end();
      M_node.end();
      M_inc.end();
    }

    void
    use(IloCplex &cplex)
    {
      if (!M_lazy_cbs.empty()) {
        cplex.use(M_lazy);
      }

      if (!M_cut_cbs.empty()) {
        cplex.use(M_cut);
      }

      if (M_branch_cb) {
        cplex.use(M_branch);
      }

      if (M_node_cb) {
        cplex.use(M_node);
      }

      if (M_inc_cb) {
        cplex.use(M_inc);
      }
    }

    void
    add_callback(void (CallingType::*cb)(lazy_constraint_callback *))
    {
      M_lazy_cbs.push_back(cb);
    }

    void
    add_callback(void (CallingType::*cb)(user_cut_callback *))
    {
      M_cut_cbs.push_back(cb);
    }

    void
    set_callback(void (CallingType::*cb)(branch_callback *))
    {
      M_branch_cb = cb;
    }

    void
    set_callback(void (CallingType::*cb)(node_callback *))
    {
      M_node_cb = cb;
    }

    void
    set_callback(void (CallingType::*cb)(incumbent_callback *))
    {
      M_inc_cb = cb;
    }

    std::chrono::microseconds
    cut_time() const
    { return M_cut_time; }

    std::chrono::microseconds
    lazy_time() const
    { return M_lazy_time; }

    std::chrono::microseconds
    branch_time() const
    { return M_branch_time; }

    std::chrono::microseconds
    node_time() const
    { return M_node_time; }

    std::chrono::microseconds
    incumbent_time() const
    { return M_inc_time; }

    std::size_t
    cut_calls() const
    { return M_cut_calls; }

    std::size_t
    lazy_calls() const
    { return M_lazy_calls; }

    std::size_t
    branch_calls() const
    { return M_branch_calls; }

    std::size_t
    node_calls() const
    { return M_node_calls; }

    std::size_t
    incumbent_calls() const
    { return M_inc_calls; }

  private:
    struct branch_cb : public branch_callback
    {
      branch_cb(IloEnv env, callback_container &cont) : branch_callback(env), cont(cont)
      {}

      IloCplex::CallbackI *
      duplicateCallback() const override
      {
        return new(this->getEnv()) branch_cb(this->getEnv(), cont);
      }

      void
      main() override
      {
        cont.call_branch(this);
      }

    private:
      callback_container &cont;
    };

    struct lazy_cb : public lazy_constraint_callback
    {
      lazy_cb(IloEnv env, callback_container &cont) : lazy_constraint_callback(env), cont(cont)
      {}

      IloCplex::CallbackI *
      duplicateCallback() const override
      {
        return (new(this->getEnv()) lazy_cb(this->getEnv(), cont));
      }

      void
      main() override
      {
        cont.call_lazy(this);
      }

    private:
      callback_container &cont;
    };

    struct cut_cb : public user_cut_callback
    {
      cut_cb(IloEnv env, callback_container &cont) : user_cut_callback(env), cont(cont)
      {}

      IloCplex::CallbackI *
      duplicateCallback() const override
      {
        return (new(this->getEnv()) cut_cb(this->getEnv(), cont));
      }

      void
      main() override
      {
        cont.call_cut(this);
      }

    private:
      callback_container &cont;
    };

    struct node_cb : public node_callback
    {
      node_cb(IloEnv env, callback_container &cont) : node_callback(env), cont(cont)
      {}

      IloCplex::CallbackI *
      duplicateCallback() const override
      {
        return (new(this->getEnv()) node_cb(this->getEnv(), cont));
      }

      void
      main() override
      {
        cont.call_node(this);
      }

    private:
      callback_container &cont;
    };

    struct inc_cb : public incumbent_callback
    {
      inc_cb(IloEnv env, callback_container &cont) : incumbent_callback(env), cont(cont)
      {}

      IloCplex::CallbackI *
      duplicateCallback() const override
      {
        return (new(this->getEnv()) inc_cb(this->getEnv(), cont));
      }

      void
      main() override
      {
        cont.call_inc(this);
      }

    private:
      callback_container &cont;
    };

    void
    call_branch(branch_callback *cbptr)
    {
      std::chrono::steady_clock::time_point tprev, tafter;

      if (M_time_branch) {
        tprev = std::chrono::steady_clock::now();
        ++M_branch_calls;
      }

      (M_calling.*M_branch_cb)(cbptr);

      if (M_time_branch) {
        tafter = std::chrono::steady_clock::now();
        M_branch_time += std::chrono::duration_cast<std::chrono::microseconds>(tafter - tprev);
      }
    }

    void
    call_lazy(lazy_constraint_callback *cbptr)
    {
      std::chrono::steady_clock::time_point tprev, tafter;

      if (M_time_lazy) {
        tprev = std::chrono::steady_clock::now();
        ++M_lazy_calls;
      }

      void
      (CallingType::*ptr)(lazy_constraint_callback *);

      for (std::size_t i = 0, s = M_lazy_cbs.size(); i < s; ++i) {
        ptr = M_lazy_cbs[i];
        (M_calling.*ptr)(cbptr);
      }

      if (M_time_lazy) {
        tafter = std::chrono::steady_clock::now();
        M_lazy_time += std::chrono::duration_cast<std::chrono::microseconds>(tafter - tprev);
      }
    }

    void
    call_cut(user_cut_callback *cbptr)
    {
      std::chrono::steady_clock::time_point tprev, tafter;

      if (M_time_cuts) {
        tprev = std::chrono::steady_clock::now();
        ++M_cut_calls;
      }

      void
      (CallingType::*ptr)(user_cut_callback *);

      for (std::size_t i = 0, s = M_cut_cbs.size(); i < s; ++i) {
        ptr = M_cut_cbs[i];
        (M_calling.*ptr)(cbptr);
      }

      if (M_time_cuts) {
        tafter = std::chrono::steady_clock::now();
        M_cut_time += std::chrono::duration_cast<std::chrono::microseconds>(tafter - tprev);
      }
    }

    void
    call_node(node_callback *cbptr)
    {
      std::chrono::steady_clock::time_point tprev, tafter;

      if (M_time_node) {
        tprev = std::chrono::steady_clock::now();
        ++M_node_calls;
      }

      (M_calling.*M_node_cb)(cbptr);

      if (M_time_node) {
        tafter = std::chrono::steady_clock::now();
        M_node_time += std::chrono::duration_cast<std::chrono::microseconds>(tafter - tprev);
      }
    }

    void
    call_inc(incumbent_callback *cbptr)
    {
      std::chrono::steady_clock::time_point tprev, tafter;

      if (M_time_inc) {
        tprev = std::chrono::steady_clock::now();
        ++M_inc_calls;
      }

      (M_calling.*M_inc_cb)(cbptr);

      if (M_time_inc) {
        tafter = std::chrono::steady_clock::now();
        M_inc_time += std::chrono::duration_cast<std::chrono::microseconds>(tafter - tprev);
      }
    }

    CallingType &M_calling;
    IloCplex::Callback M_cut, M_lazy, M_branch, M_node, M_inc;
    std::vector<void (CallingType::*)(lazy_constraint_callback *)> M_lazy_cbs;
    std::vector<void (CallingType::*)(user_cut_callback *)> M_cut_cbs;

    void
    (CallingType::*M_branch_cb)(branch_callback *);

    void
    (CallingType::*M_node_cb)(node_callback *);

    void
    (CallingType::*M_inc_cb)(incumbent_callback *);

    bool M_time_cuts, M_time_lazy, M_time_branch, M_time_node, M_time_inc;
    std::chrono::microseconds M_cut_time, M_lazy_time, M_branch_time, M_node_time, M_inc_time;
    std::size_t M_cut_calls, M_lazy_calls, M_branch_calls, M_node_calls, M_inc_calls;
  };

  struct callback_times
  {
    callback_times() :
            user_cut_time(0),
            lazy_constraint_time(0),
            branch_time(0),
            node_time(0),
            incumbent_time(0),
            user_cut_calls(0),
            lazy_constraint_calls(0),
            branch_calls(0),
            node_calls(0),
            incumbent_calls(0)
    {}

    template<typename CBC>
    callback_times(const callback_container<CBC> &cont) :
            user_cut_time(cont.cut_time()),
            lazy_constraint_time(cont.lazy_time()),
            branch_time(cont.branch_time()),
            node_time(cont.node_time()),
            incumbent_time(cont.incumbent_time()),
            user_cut_calls(cont.cut_calls()),
            lazy_constraint_calls(cont.lazy_calls()),
            branch_calls(cont.branch_calls()),
            node_calls(cont.node_calls()),
            incumbent_calls(cont.incumbent_calls())
    {}

    explicit operator bool() const
    {
      return user_cut_time.count() || lazy_constraint_time.count() || branch_time.count() || node_time.count() ||
             incumbent_time.count();
    }

    bool
    operator!() const
    {
      return !static_cast<bool>(*this);
    }

    std::chrono::microseconds user_cut_time, lazy_constraint_time, branch_time, node_time, incumbent_time;
    std::size_t user_cut_calls, lazy_constraint_calls, branch_calls, node_calls, incumbent_calls;

    friend std::ostream &
    operator<<(std::ostream &ostr, const callback_times &times)
    {
      using std::chrono::duration_cast;

      typedef std::chrono::duration<double> secs;

      if (times.user_cut_time.count() != 0) {
        ostr << "User Cut Callback: " << duration_cast<secs>(times.user_cut_time).count() << " s ("
             << times.user_cut_calls << " calls)" << std::endl;
      }

      if (times.lazy_constraint_time.count() != 0) {
        ostr << "Lazy Constraint Callback: " << duration_cast<secs>(times.lazy_constraint_time).count() << " s ("
             << times.lazy_constraint_calls << " calls)" << std::endl;
      }

      if (times.branch_time.count() != 0) {
        ostr << "Branch Callback: " << duration_cast<secs>(times.branch_time).count() << " s (" << times.branch_calls
             << " calls)" << std::endl;
      }

      if (times.node_time.count() != 0) {
        ostr << "Node Callback: " << duration_cast<secs>(times.node_time).count() << " s (" << times.node_calls
             << " calls)" << std::endl;
      }

      if (times.incumbent_time.count() != 0) {
        ostr << "Incumbent Callback: " << duration_cast<secs>(times.incumbent_time).count() << " s ("
             << times.incumbent_calls << " calls)" << std::endl;
      }

      return ostr;
    }
  };

  struct extractable_compare
  {
    bool
    operator()(const IloExtractable &e1, const IloExtractable &e2) const noexcept
    {
      return e1.getImpl() == e2.getImpl();
    }
  };

  struct extractable_hash
  {
    std::size_t
    operator()(const IloExtractable &e) const noexcept
    {
      const std::hash<IloExtractableI *> hasher{};
      return hasher(e.getImpl());
    }
  };

  CPLEX_HPP_NORETURN inline void
  wrap_and_throw_cplex_exception(IloException &ex, const char *catching_file, const char *catching_func,
                                 unsigned catching_line)
  {
    std::ostringstream message;

    if (dynamic_cast<IloAlgorithm::CannotChangeException *>(&ex)) {
      IloAlgorithm::CannotChangeException &e = dynamic_cast<IloAlgorithm::CannotChangeException &>(ex);
      IloExtractableArray &extractables = e.getExtractables();

      message << "IloAlgorithm::CannotChangeException: " << ex.getMessage() << std::endl;

      message << "Extractables: ";
      for (std::size_t i = 0, s = extractables.getSize(); i < s; ++i) {
        message << extractables[i] << " ";
      }
      message << std::endl;
    } else if (dynamic_cast<IloAlgorithm::NotExtractedException *>(&ex)) {
      message << "IloAlgorithm::NotExtractedException: " << ex.getMessage() << std::endl;
    } else if (dynamic_cast<IloAlgorithm::CannotExtractException *>(&ex)) {
      message << "IloAlgorithm::CannotExtractException: " << ex.getMessage() << std::endl;
    } else {
      message << "IloException: " << ex.getMessage() << std::endl;
    }

    message << "Caught in function " << catching_func << ", in '" << catching_file << "':" << catching_line;
    throw std::runtime_error(message.str());
  }

  namespace impl
  {
    template<typename T, typename... Args>
    struct addresult_or_void
    {
    private:
      struct addresult_or_void_impl
      {
        void
        add(...)
        {}

        template<typename... Args2>
        auto
        add(Args2 &&... args) -> decltype(std::declval<T &>().add(std::forward<Args2>(args)...))
        {}
      };

    public:
      typedef decltype(std::declval<addresult_or_void_impl>().add(std::forward<Args>(std::declval<Args>())...)) type;
    };

    template<typename T, typename Arg>
    struct indexresult_or_void
    {
    private:
      struct indexresult_or_void_impl
      {
        void
        index(...)
        {}

        template<typename Arg2>
        auto
        index(Arg2 &&arg) -> decltype(std::declval<T &>()[std::forward<Arg2>(arg)])
        {}
      };

    public:
      typedef decltype(std::declval<indexresult_or_void_impl>().index(std::forward<Arg>(std::declval<Arg>()))) type;
    };

    template<typename Wrapped, bool EndElements>
    struct wrap
    {
    public:
      template<typename... Args>
      wrap(Args &&... args) :
              M_wrapped(std::forward<Args>(args)...)
      {}

      wrap(wrap &&o) : M_wrapped(o.M_wrapped)
      {
        o.M_wrapped = Wrapped();
      }

      wrap &
      operator=(wrap &&o)
      {
        if (&o != this) {
          P_check_destroy();
          M_wrapped = o.M_wrapped;
          o.M_wrapped = Wrapped();
        }
        return *this;
      }

      ~wrap()
      { P_check_destroy(); }

      Wrapped *
      operator->() noexcept
      { return &M_wrapped; }

      const Wrapped *
      operator->() const noexcept
      { return &M_wrapped; }

      operator Wrapped &() noexcept
      { return M_wrapped; }

      operator const Wrapped &() const noexcept
      { return M_wrapped; }

      Wrapped &
      operator*() noexcept
      { return M_wrapped; }

      const Wrapped &
      operator*() const noexcept
      { return M_wrapped; }

      template<typename... Args>
      typename addresult_or_void<Wrapped, Args...>::type
      add(Args &&... args)
      {
        return M_wrapped.add(std::forward<Args>(args)...);
      }

      template<typename Arg>
      typename indexresult_or_void<Wrapped, Arg>::type
      operator[](Arg &&arg)
      {
        return M_wrapped[std::forward<Arg>(arg)];
      }

    private:
      wrap(const wrap &) = delete;

      wrap &
      operator=(const wrap &) = delete;

      void
      P_check_destroy()
      {
        if (M_wrapped.getImpl()) {
          P_call_end_elements<EndElements>();
          M_wrapped.end();
        }
      }

      template<bool B>
      typename std::enable_if<B, void>::type
      P_call_end_elements()
      {
        M_wrapped.endElements();
      }

      template<bool B>
      typename std::enable_if<!B, void>::type
      P_call_end_elements()
      {}

      Wrapped M_wrapped;
    };
  }

  template<typename T> using wrap = impl::wrap<T, false>;
  template<typename T> using wrap_array = impl::wrap<T, true>;
}

#define WRAP_AND_THROW_CPLEX_EXCEPTION(ex) ::cpxhelper::wrap_and_throw_cplex_exception(ex, __FILE__, __PRETTY_FUNCTION__, __LINE__)

#undef CPLEX_HPP_NORETURN

#endif //IP_DISPERSION_CPLEX_HPP
