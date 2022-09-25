#define STATIC_ASSERT(Expr) typedef int static_assert_typedef[(!!(Expr)) ? 1 : -1]
#define ASSERT(Expr) if(Expr); else DEBUG_BREAK()

#define DEFAULT_WINDOW_WIDTH  800
#define DEFAULT_WINDOW_HEIGHT 600