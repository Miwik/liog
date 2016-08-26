#ifndef LIOG
#define LIOG

#include <iostream>
#include <iomanip> // put_time
#include <type_traits>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// general configuration
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define THREAD_SAFE_LIOG 1

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// user friendly macros
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// log messages
#define liog(...) CAT(_CALL_EMPTY_LOG_, ISEMPTY(__VA_ARGS__))(__VA_ARGS__)

// custom messages creation
#define liog_create(...) CAT(_CALL_EMPTY_CREATE_, ISEMPTY(__VA_ARGS__))(__VA_ARGS__)
#define liog_set(NAME)   static constexpr const bool _has_##NAME = true; static constexpr decltype(config::t_Default::NAME) NAME
#define liog_end         }; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dark magic
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if THREAD_SAFE_LIOG
#  include <mutex>
namespace nliog {
namespace _private {
    static std::mutex gLiogMutex;
} // liog
} // _private
#endif

#define STRINGIFY(A_STRING) #A_STRING
#define GLUE(A_STRING, B_STRING) A_STRING##B_STRING

#define CAT(a, ...) _PRIMITIVE_CAT1(a, __VA_ARGS__)
#define _PRIMITIVE_CAT1(a, ...) a##__VA_ARGS__

// merge values of multiple configs
#define _GENERATE_GET_VALUE(MEMBER) \
    /* general recursive case */ \
    template<bool hasmember, typename /*prev config*/, typename Config, typename... Configs> \
    struct _get_##MEMBER##_value_impl { \
    static constexpr bool _has_##MEMBER = _get_##MEMBER##_value_impl<Config::_has_##MEMBER, Config, Configs...>::_has_##MEMBER; \
    static constexpr decltype(nliog::config::t_Default::MEMBER) MEMBER = \
    _get_##MEMBER##_value_impl<Config::_has_##MEMBER, Config, Configs...>::MEMBER; }; \
    /* we got a value in a config */ \
    template<typename Config, typename Eat, typename... Configs> \
    struct _get_##MEMBER##_value_impl<true, Config, Eat, Configs...> { \
    static constexpr bool _has_##MEMBER = true; \
    static constexpr decltype(nliog::config::t_Default::MEMBER) MEMBER = Config::MEMBER; }; \
    /* define nothing for NullType */ \
    template<bool hasmember, typename Eat, typename... Configs> \
    struct _get_##MEMBER##_value_impl<hasmember, NullType, Eat, Configs...> { \
    static constexpr bool _has_##MEMBER = false; }; \
    /* entry point with default params to call the general recursive case */ \
    template<typename... Configs> \
    struct _get_##MEMBER##_value : _get_##MEMBER##_value_impl<false, Empty, Configs..., NullType, Empty /*placeholder*/> { }

#define _ARG64( \
    _0,   _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, \
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, \
    _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, \
    _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, \
    _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, \
    _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, \
    _60, _61, _62, _63, ...) _63

#define HAS_COMMA(...) _ARG64(__VA_ARGS__, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 0)

#define _VA_NUM_ARGS(...) _ARG64(__VA_ARGS__, \
    63, 62, 61, 60,                   \
    59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
    49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
    39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
    29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
    19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
    9,   8,  7,  6,  5,  4,  3,  2,  1,  0 /* 0 will be the comma if __VA_ARGS__ is empty, so an empty varargs will evaluate to 1 */)

#define _TRIGGER_PARENTHESIS_(...) ,

#define ISEMPTY(...) _ISEMPTY(                                                              \
    /* test if there is just one argument, eventually an empty one */                       \
    HAS_COMMA(__VA_ARGS__),                                                                 \
    /* test if _TRIGGER_PARENTHESIS_ together with the argument adds a comma */             \
    HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__),                                           \
    /* test if the argument together with a parenthesis adds a comma */                     \
    HAS_COMMA(__VA_ARGS__ (/*empty*/)),                                                     \
    /* test if placing it between _TRIGGER_PARENTHESIS_ and the parenthesis adds a comma */ \
    HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__ (/*empty*/))                                \
    )

#define _PASTE5(_0, _1, _2, _3, _4) _0##_1##_2##_3##_4
#define _ISEMPTY(_0, _1, _2, _3) HAS_COMMA(_PASTE5(_IS_EMPTY_CASE_, _0, _1, _2, _3))
#define _IS_EMPTY_CASE_0001 ,

#define _VA_COUNT_ARGS_0(...) _VA_NUM_ARGS(__VA_ARGS__)
#define _VA_COUNT_ARGS_1(...) 0 /* is empty */
#define VA_COUNT_ARGS(...) CAT(_VA_COUNT_ARGS_, ISEMPTY(__VA_ARGS__))(__VA_ARGS__)

#define _CALL_EMPTY_LOG_1(...) _log0() /* empty */
#define _CALL_EMPTY_LOG_0(...) _ARG64(__VA_ARGS__, \
    _log2, _log2, _log2, _log2, _log2, _log2, _log2, _log2, _log2, _log2, \
    _log2, _log2, _log2, _log2, _log2, _log2, _log2, _log2, _log2, _log2, \
    _log2, _log2, _log2, _log2, _log2, _log2, _log2, _log2, _log2, _log2, \
    _log2, _log2, _log2, _log2, _log2, _log2, _log2, _log2, _log2, _log2, \
    _log2, _log2, _log2, _log2, _log2, _log2, _log2, _log2, _log2, _log2, \
    _log2, _log2, _log2, _log2, _log2, _log2, _log2, _log2, _log2, _log2, \
    _log2, _log2, _log1)(__VA_ARGS__) /* not empty :D */

#define _liog_config_name(NAME) GLUE(_liog_config_name_, NAME)

#if !defined(_NOLIOG)

#if THREAD_SAFE_LIOG

// no arguments, set a default config and no messages
#define _log0() \
    do { \
    using namespace nliog; \
    std::lock_guard<std::mutex> lock(_private::gLiogMutex); \
    tliog_entry_point<config::t_Default>(__TIME__, __DATE__, __FILE__, __LINE__, __func__); \
    } while (0)

// 1 argument, must be a config without messages
#define _log1(CONFIG) \
    do { \
    using namespace nliog; \
    std::lock_guard<std::mutex> lock(_private::gLiogMutex); \
    tliog_entry_point<_liog_config_name(CONFIG)>(__TIME__, __DATE__, __FILE__, __LINE__, __func__); \
    } while (0)

// 2 arguments or more, config + messages
#define _log2(CONFIG, ...) \
    do { \
    using namespace nliog; \
    std::lock_guard<std::mutex> lock(_private::gLiogMutex); \
    tliog_entry_point<_liog_config_name(CONFIG)>(__TIME__, __DATE__, __FILE__, __LINE__, __func__, __VA_ARGS__); \
    } while (0)

#else // no thread safe

// no arguments, set a default config and no messages
#define _log0() \
    do { \
    using namespace nliog; \
    tliog_entry_point<config::t_Default>(__TIME__, __DATE__, __FILE__, __LINE__, __func__); \
    } while (0)

// 1 argument, must be a config without messages
#define _log1(CONFIG) \
    do { \
    using namespace nliog; \
    tliog_entry_point<_liog_config_name(CONFIG)>(__TIME__, __DATE__, __FILE__, __LINE__, __func__); \
    } while (0)

// 2 arguments or more, config + messages
#define _log2(CONFIG, ...) \
    do { \
    using namespace nliog; \
    tliog_entry_point<_liog_config_name(CONFIG)>(__TIME__, __DATE__, __FILE__, __LINE__, __func__, __VA_ARGS__); \
    } while (0)

#endif

#else // log not active

#define _log0()            do { } while (0)
#define _log1(CONFIG)      do { } while (0)
#define _log2(CONFIG, ...) do { } while (0)

#endif

#define _CALL_EMPTY_CREATE_1(...) _liog_create0() /* empty */
#define _CALL_EMPTY_CREATE_0(...) _ARG64(__VA_ARGS__, \
    _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, \
    _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, \
    _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, \
    _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, \
    _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, \
    _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, \
    _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create2, _liog_create1\
    )(__VA_ARGS__) /* not empty :D */

// allow 64 configs to be merged
#define _BUILD_CONF_LIST(...) CAT(_BUILD_CONF_, VA_COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)
#define _BUILD_CONF_1(CONF)       , _liog_config_name(build_##CONF)
#define _BUILD_CONF_2(CONF, ...)  , _liog_config_name(build_##CONF) _BUILD_CONF_1(__VA_ARGS__)
#define _BUILD_CONF_3(CONF, ...)  , _liog_config_name(build_##CONF) _BUILD_CONF_2(__VA_ARGS__)
#define _BUILD_CONF_4(CONF, ...)  , _liog_config_name(build_##CONF) _BUILD_CONF_3(__VA_ARGS__)
#define _BUILD_CONF_5(CONF, ...)  , _liog_config_name(build_##CONF) _BUILD_CONF_4(__VA_ARGS__)
#define _BUILD_CONF_6(CONF, ...)  , _liog_config_name(build_##CONF) _BUILD_CONF_5(__VA_ARGS__)
#define _BUILD_CONF_7(CONF, ...)  , _liog_config_name(build_##CONF) _BUILD_CONF_6(__VA_ARGS__)
#define _BUILD_CONF_8(CONF, ...)  , _liog_config_name(build_##CONF) _BUILD_CONF_7(__VA_ARGS__)
#define _BUILD_CONF_9(CONF, ...)  , _liog_config_name(build_##CONF) _BUILD_CONF_8(__VA_ARGS__)
#define _BUILD_CONF_10(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_9(__VA_ARGS__)
#define _BUILD_CONF_11(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_10(__VA_ARGS__)
#define _BUILD_CONF_12(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_11(__VA_ARGS__)
#define _BUILD_CONF_13(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_12(__VA_ARGS__)
#define _BUILD_CONF_14(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_13(__VA_ARGS__)
#define _BUILD_CONF_15(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_14(__VA_ARGS__)
#define _BUILD_CONF_16(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_15(__VA_ARGS__)
#define _BUILD_CONF_17(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_16(__VA_ARGS__)
#define _BUILD_CONF_18(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_17(__VA_ARGS__)
#define _BUILD_CONF_19(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_18(__VA_ARGS__)
#define _BUILD_CONF_20(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_19(__VA_ARGS__)
#define _BUILD_CONF_21(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_20(__VA_ARGS__)
#define _BUILD_CONF_22(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_21(__VA_ARGS__)
#define _BUILD_CONF_23(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_22(__VA_ARGS__)
#define _BUILD_CONF_24(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_23(__VA_ARGS__)
#define _BUILD_CONF_25(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_24(__VA_ARGS__)
#define _BUILD_CONF_26(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_25(__VA_ARGS__)
#define _BUILD_CONF_27(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_26(__VA_ARGS__)
#define _BUILD_CONF_28(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_27(__VA_ARGS__)
#define _BUILD_CONF_29(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_28(__VA_ARGS__)
#define _BUILD_CONF_30(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_29(__VA_ARGS__)
#define _BUILD_CONF_31(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_30(__VA_ARGS__)
#define _BUILD_CONF_32(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_31(__VA_ARGS__)
#define _BUILD_CONF_33(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_32(__VA_ARGS__)
#define _BUILD_CONF_34(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_33(__VA_ARGS__)
#define _BUILD_CONF_35(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_34(__VA_ARGS__)
#define _BUILD_CONF_36(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_35(__VA_ARGS__)
#define _BUILD_CONF_37(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_36(__VA_ARGS__)
#define _BUILD_CONF_38(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_37(__VA_ARGS__)
#define _BUILD_CONF_39(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_38(__VA_ARGS__)
#define _BUILD_CONF_40(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_39(__VA_ARGS__)
#define _BUILD_CONF_41(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_40(__VA_ARGS__)
#define _BUILD_CONF_42(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_41(__VA_ARGS__)
#define _BUILD_CONF_43(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_42(__VA_ARGS__)
#define _BUILD_CONF_44(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_43(__VA_ARGS__)
#define _BUILD_CONF_45(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_44(__VA_ARGS__)
#define _BUILD_CONF_46(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_45(__VA_ARGS__)
#define _BUILD_CONF_47(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_46(__VA_ARGS__)
#define _BUILD_CONF_48(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_47(__VA_ARGS__)
#define _BUILD_CONF_49(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_48(__VA_ARGS__)
#define _BUILD_CONF_50(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_49(__VA_ARGS__)
#define _BUILD_CONF_51(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_50(__VA_ARGS__)
#define _BUILD_CONF_52(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_51(__VA_ARGS__)
#define _BUILD_CONF_53(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_52(__VA_ARGS__)
#define _BUILD_CONF_54(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_53(__VA_ARGS__)
#define _BUILD_CONF_55(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_54(__VA_ARGS__)
#define _BUILD_CONF_56(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_55(__VA_ARGS__)
#define _BUILD_CONF_57(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_56(__VA_ARGS__)
#define _BUILD_CONF_58(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_57(__VA_ARGS__)
#define _BUILD_CONF_59(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_58(__VA_ARGS__)
#define _BUILD_CONF_60(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_59(__VA_ARGS__)
#define _BUILD_CONF_61(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_60(__VA_ARGS__)
#define _BUILD_CONF_62(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_61(__VA_ARGS__)
#define _BUILD_CONF_63(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_62(__VA_ARGS__)
#define _BUILD_CONF_64(CONF, ...) , _liog_config_name(build_##CONF) _BUILD_CONF_63(__VA_ARGS__)

// no arguments; override the base config
#define _liog_create0() \
    namespace nliog { \
    struct _liog_config_name(DefaultOverride); \
    namespace traits { \
    template<> struct is_config<_liog_config_name(DefaultOverride)> { static constexpr bool value = true; }; \
    template<> struct is_default_config_overriden<config::t_Default> { static constexpr bool value = true; }; } \
    struct _liog_config_name(DefaultOverride) : config::t_Default { \
    static constexpr const char* conf_name = "DefaultOverrride";

// 1 argument; the configuration's name
#define _liog_create1(NAME) \
    namespace nliog { \
    /* build the complete config properties */ \
    struct _liog_config_name(partial_##NAME); \
    struct _liog_config_name(build_##NAME) : _private::GenerateConfig<_liog_config_name(partial_##NAME)> { }; \
    struct _liog_config_name(NAME) : _private::GenerateConfig<_liog_config_name(build_##NAME), \
    typename traits::select_config<config::t_Default>::result> { }; \
    namespace traits { template<> struct is_config<_liog_config_name(NAME)> { static constexpr bool value = true; }; } \
    /* local properties */ \
    struct _liog_config_name(partial_##NAME) : _private::s_has_member<false> { \
    static constexpr bool _has_conf_name = true; static constexpr const char* conf_name = STRINGIFY(NAME);

// 2 or more arguments; the 1st is the configuration's name, the others are configs to merge with
#define _liog_create2(NAME, ...) \
    namespace nliog { \
    /* build the complete config properties */ \
    struct _liog_config_name(partial_##NAME); \
    struct _liog_config_name(build_##NAME) : _private::GenerateConfig<_liog_config_name(partial_##NAME) \
    _BUILD_CONF_LIST(__VA_ARGS__)> { }; \
    struct _liog_config_name(NAME) : _private::GenerateConfig<_liog_config_name(build_##NAME) \
    _BUILD_CONF_LIST(__VA_ARGS__), \
    typename traits::select_config<config::t_Default>::result> { }; \
    namespace traits { template<> struct is_config<_liog_config_name(NAME)> { static constexpr bool value = true; }; } \
    /* local properties */ \
    struct _liog_config_name(partial_##NAME) : _private::s_has_member<false> { \
    static constexpr bool _has_conf_name = true; static constexpr const char* conf_name = STRINGIFY(NAME);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// liog core
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace nliog {

enum class fcolor { black, red, green, yellow, blue, magenta, cyan, white, original, current         };
enum class bcolor { black, red, green, yellow, blue, magenta, cyan, white, original, current         };
enum class style  { none, bright, dark, italic, underlined, reverse_color, hidden, original, current };

template<typename Enum>
typename std::underlying_type<Enum>::type as_integer(const Enum value) {
    static_assert(std::is_enum<Enum>::value, "Parameter is not of type enum or enum class!");
    return static_cast<typename std::underlying_type<Enum>::type>(value);
}

struct font {

    font(fcolor _fcolor = fcolor::original, bcolor _bcolor = bcolor::original, style _style = style::original) :
        m_fcolor(_fcolor), m_bcolor(_bcolor), m_style(_style) { }

    inline font& operator=(const font& other) {
        if (this == &other)                    { return *this;              }
        if (other.m_fcolor != fcolor::current) { m_fcolor = other.m_fcolor; }
        if (other.m_bcolor != bcolor::current) { m_bcolor = other.m_bcolor; }
        if (other.m_style != style::current)   { m_style = other.m_style;   }
        return *this;
    }

    fcolor m_fcolor;
    bcolor m_bcolor;
    style  m_style;
};

inline bool operator==(const font& current_font, const font& temporary_font) {
    return ((temporary_font.m_fcolor == fcolor::current || current_font.m_fcolor == temporary_font.m_fcolor) &&
            (temporary_font.m_bcolor == bcolor::current || current_font.m_bcolor == temporary_font.m_bcolor) &&
            (temporary_font.m_style == style::current || current_font.m_style == temporary_font.m_style));
}

inline bool operator!=(const font& current_font, const font& temporary_font) {
    return !operator==(current_font, temporary_font);
}

namespace traits {

template<typename T> struct is_font_attribute         { static constexpr bool value = false; };
template<>           struct is_font_attribute<fcolor> { static constexpr bool value = true;  };
template<>           struct is_font_attribute<bcolor> { static constexpr bool value = true;  };
template<>           struct is_font_attribute<style>  { static constexpr bool value = true;  };
template<>           struct is_font_attribute<font>   { static constexpr bool value = true;  };

} // traits

namespace _private {

// put the resetfont outside the enum classes so it keep the font code simple w/o special cases, hum origin...
struct resetfont_type { };
// font currently applied
static font current_font;
// font used by liog and applied to current font when valid
static font temporary_font;

} // _private

static constexpr _private::resetfont_type resetfont; // reset to original / default

// this is the only method to port on other systems
inline std::ostream& operator<<(std::ostream& os, const font& temporary_font) {
    // construct the font code: \033[{attribute};{foreground};{background}m
    // attribute          // foreground         // background
    // 0 - nothing        // 30 black           // 40 black
    // 1 - bright         // 31 red             // 41 red
    // 2 - dark           // 32 green           // 42 green
    // 3 - italic         // 33 yellow          // 43 yellow
    // 4 - underlined     // 34 blue            // 44 blue
    // 7 - reverse color  // 35 magenta         // 45 magenta
    // 8 - hidden         // 36 cyan            // 46 cyan
                          // 37 gray            // 47 gray

    // no need to apply the color code if we already have the same current_font
    if (_private::current_font == temporary_font) {
        return os;
    }
    // the temporary font is different from the current_font, set the current_font to the temporary value
    _private::current_font = temporary_font; // temporary_font current values are ignored
    // reset the font to default if we have an original color
    if (_private::current_font.m_fcolor == fcolor::original || _private::current_font.m_bcolor == bcolor::original ||
            _private::current_font.m_style == style::original) {
        os << "\033[0m";
    }
    // all font component are requested as original, we are over
    if (_private::current_font.m_fcolor == fcolor::original && _private::current_font.m_bcolor == bcolor::original &&
            _private::current_font.m_style == style::original) {
        return os;
    }
    // update the temporary font: replace all 'current' tags with real current values
    _private::temporary_font = _private::current_font;
    // construct the font code and write it to the output
    os << "\033[";
    if (_private::current_font.m_style != style::original) {
        os << (as_integer(_private::current_font.m_style) <= 4 ?
                   as_integer(_private::current_font.m_style) :
                   as_integer(_private::current_font.m_style) + 2);
        if (_private::current_font.m_fcolor != fcolor::original || _private::current_font.m_bcolor != bcolor::original) {
            os << ";";
        }
    }
    if (_private::current_font.m_fcolor != fcolor::original) {
        os << std::to_string(30 + as_integer(_private::current_font.m_fcolor));
        if (_private::current_font.m_bcolor != bcolor::original) {
            os << ";";
        }
    }
    if (_private::current_font.m_bcolor != bcolor::original) {
        os << std::to_string(40 + as_integer(_private::current_font.m_bcolor));
    }
    return os << "m";
}

inline std::ostream& operator<<(std::ostream& os, const _private::resetfont_type&) {
    _private::temporary_font.m_fcolor = fcolor::original;
    _private::temporary_font.m_bcolor = bcolor::original;
    _private::temporary_font.m_style = style::original;
    return os << _private::temporary_font;
}

inline std::ostream& operator<<(std::ostream& os, fcolor _fcolor) {
    _private::temporary_font.m_fcolor = _fcolor;
    return os <<_private::temporary_font;
}

inline std::ostream& operator<<(std::ostream& os, bcolor _bcolor) {
    _private::temporary_font.m_bcolor = _bcolor;
    return os << _private::temporary_font;
}

inline std::ostream& operator<<(std::ostream& os, style _style) {
    _private::temporary_font.m_style = _style;
    return os << _private::temporary_font;
}

namespace _private {

#define _CONFIG_PROPERTIES(VALUE) \
    static constexpr bool _has_conf_name = VALUE; \
    /* date & time */ \
    static constexpr bool _has_dt_date_display = VALUE; \
    static constexpr bool _has_dt_time_display = VALUE; \
    static constexpr bool _has_dt_prefix = VALUE; \
    static constexpr bool _has_dt_suffix = VALUE; \
    static constexpr bool _has_dt_delimiters_fcolor = VALUE; \
    static constexpr bool _has_dt_delimiters_bcolor = VALUE; \
    static constexpr bool _has_dt_delimiters_style = VALUE; \
    static constexpr bool _has_dt_separator = VALUE; \
    static constexpr bool _has_dt_fcolor = VALUE; \
    static constexpr bool _has_dt_bcolor = VALUE; \
    static constexpr bool _has_dt_style = VALUE; \
    /* tag */ \
    static constexpr bool _has_tag = VALUE; \
    static constexpr bool _has_tag_display = VALUE; \
    static constexpr bool _has_tag_prefix = VALUE; \
    static constexpr bool _has_tag_suffix = VALUE; \
    static constexpr bool _has_tag_delimiters_fcolor = VALUE; \
    static constexpr bool _has_tag_delimiters_bcolor = VALUE; \
    static constexpr bool _has_tag_delimiters_style = VALUE; \
    static constexpr bool _has_tag_fcolor = VALUE; \
    static constexpr bool _has_tag_bcolor = VALUE; \
    static constexpr bool _has_tag_style = VALUE; \
    /*infos*/ \
    static constexpr bool _has_infos_display = VALUE; \
    static constexpr bool _has_infos_prefix = VALUE; \
    static constexpr bool _has_infos_suffix = VALUE; \
    static constexpr bool _has_infos_delimiters_fcolor = VALUE; \
    static constexpr bool _has_infos_delimiters_bcolor = VALUE; \
    static constexpr bool _has_infos_delimiters_style = VALUE; \
    static constexpr bool _has_infos_separator = VALUE; \
    static constexpr bool _has_infos_display_function = VALUE; \
    static constexpr bool _has_infos_display_file = VALUE; \
    static constexpr bool _has_infos_display_line = VALUE; \
    static constexpr bool _has_infos_function_prefix = VALUE; \
    static constexpr bool _has_infos_file_prefix = VALUE; \
    static constexpr bool _has_infos_line_prefix = VALUE; \
    static constexpr bool _has_infos_fcolor = VALUE; \
    static constexpr bool _has_infos_bcolor = VALUE; \
    static constexpr bool _has_infos_style = VALUE; \
    /*message*/ \
    static constexpr bool _has_msg_prefix = VALUE; \
    static constexpr bool _has_msg_prefix_fcolor = VALUE; \
    static constexpr bool _has_msg_prefix_bcolor = VALUE; \
    static constexpr bool _has_msg_prefix_style = VALUE; \
    static constexpr bool _has_msg_fcolor = VALUE; \
    static constexpr bool _has_msg_bcolor = VALUE; \
    static constexpr bool _has_msg_style = VALUE; \
    static constexpr bool _has_msg_values_fcolor = VALUE; \
    static constexpr bool _has_msg_values_bcolor = VALUE; \
    static constexpr bool _has_msg_values_style = VALUE; \
    static constexpr bool _has_msg_append_newline = VALUE

template<bool has> struct s_has_member       { _CONFIG_PROPERTIES(false); };
template<>         struct s_has_member<true> { _CONFIG_PROPERTIES(true);  };

struct Empty { };
struct NullType : s_has_member<false> { };

} // _private

namespace config {

// default configuration
// use nullptr when the string is empty so we can detect it with a simple comparaison and avoid to use functions such as strlen
// ex: tag_prefix = "" will apply the color code since we don't call strlen to know if the string is empty
struct Default : _private::s_has_member<true> { // [tag][infos]message
    static constexpr const char* conf_name =
            "Default";
    static constexpr const char* conf_description =
            "Default configuration used when members are not specified by custom configs."; // TODO
    // out
    //    static constexpr std::ostream& output = std::cout; // working
    static constexpr bool        output_stdout = true; // not implemented
    static constexpr bool        output_stderr = false; // not implemented
    static constexpr const char* output_file = nullptr; // not implemented
    // date and time
    static constexpr bool        dt_date_display = false;
    static constexpr bool        dt_time_display = false;
    static constexpr const char* dt_prefix = "[";
    static constexpr const char* dt_suffix = "]";
    static constexpr fcolor      dt_delimiters_fcolor = fcolor::original;
    static constexpr bcolor      dt_delimiters_bcolor = bcolor::original;
    static constexpr style       dt_delimiters_style  = style::original;
    static constexpr const char* dt_separator = " - ";
    static constexpr fcolor      dt_fcolor = fcolor::original;
    static constexpr bcolor      dt_bcolor = bcolor::original;
    static constexpr style       dt_style  = style::original;
    // tag
    static constexpr const char* tag = nullptr;
    static constexpr bool        tag_display = true;
    static constexpr const char* tag_prefix = "[";
    static constexpr const char* tag_suffix = "]";
    static constexpr fcolor      tag_delimiters_fcolor = fcolor::original;
    static constexpr bcolor      tag_delimiters_bcolor = bcolor::original;
    static constexpr style       tag_delimiters_style  = style::original;
    static constexpr fcolor      tag_fcolor = fcolor::original;
    static constexpr bcolor      tag_bcolor = bcolor::original;
    static constexpr style       tag_style  = style::original;
    // infos
    static constexpr bool        infos_display = true;
    static constexpr const char* infos_prefix = "(";
    static constexpr const char* infos_suffix = ")";
    static constexpr fcolor      infos_delimiters_fcolor = fcolor::original;
    static constexpr bcolor      infos_delimiters_bcolor = bcolor::original;
    static constexpr style       infos_delimiters_style  = style::original;
    static constexpr const char* infos_separator = " ";
    static constexpr bool        infos_display_function = true;
    static constexpr bool        infos_display_file     = true;
    static constexpr bool        infos_display_line     = true;
    static constexpr const char* infos_function_prefix = nullptr;
    static constexpr const char* infos_file_prefix     = "in "; // prefix colors? default = current
    static constexpr const char* infos_line_prefix     = "at l.";
    static constexpr fcolor      infos_fcolor = fcolor::original;
    static constexpr bcolor      infos_bcolor = bcolor::original;
    static constexpr style       infos_style  = style::original;
    // message
    static constexpr const char* msg_prefix = " ";
    static constexpr fcolor      msg_prefix_fcolor = fcolor::original;
    static constexpr bcolor      msg_prefix_bcolor = bcolor::original;
    static constexpr style       msg_prefix_style = style::original;
    static constexpr fcolor      msg_fcolor = fcolor::original;
    static constexpr bcolor      msg_bcolor = bcolor::original;
    static constexpr style       msg_style  = style::original;
    static constexpr fcolor      msg_values_fcolor = fcolor::current;
    static constexpr bcolor      msg_values_bcolor = bcolor::current;
    static constexpr style       msg_values_style  = style::current;
    static constexpr bool        msg_append_newline = true;
};

typedef Default t_Default;

} // config

namespace _private {

_GENERATE_GET_VALUE(conf_name);
// date & time
_GENERATE_GET_VALUE(dt_date_display);
_GENERATE_GET_VALUE(dt_time_display);
_GENERATE_GET_VALUE(dt_prefix);
_GENERATE_GET_VALUE(dt_suffix);
_GENERATE_GET_VALUE(dt_delimiters_fcolor);
_GENERATE_GET_VALUE(dt_delimiters_bcolor);
_GENERATE_GET_VALUE(dt_delimiters_style);
_GENERATE_GET_VALUE(dt_separator);
_GENERATE_GET_VALUE(dt_fcolor);
_GENERATE_GET_VALUE(dt_bcolor);
_GENERATE_GET_VALUE(dt_style);
// tag
_GENERATE_GET_VALUE(tag);
_GENERATE_GET_VALUE(tag_display);
_GENERATE_GET_VALUE(tag_prefix);
_GENERATE_GET_VALUE(tag_suffix);
_GENERATE_GET_VALUE(tag_delimiters_fcolor);
_GENERATE_GET_VALUE(tag_delimiters_bcolor);
_GENERATE_GET_VALUE(tag_delimiters_style);
_GENERATE_GET_VALUE(tag_fcolor);
_GENERATE_GET_VALUE(tag_bcolor);
_GENERATE_GET_VALUE(tag_style);
// infos
_GENERATE_GET_VALUE(infos_display);
_GENERATE_GET_VALUE(infos_prefix);
_GENERATE_GET_VALUE(infos_suffix);
_GENERATE_GET_VALUE(infos_delimiters_fcolor);
_GENERATE_GET_VALUE(infos_delimiters_bcolor);
_GENERATE_GET_VALUE(infos_delimiters_style);
_GENERATE_GET_VALUE(infos_separator);
_GENERATE_GET_VALUE(infos_display_function);
_GENERATE_GET_VALUE(infos_display_file);
_GENERATE_GET_VALUE(infos_display_line);
_GENERATE_GET_VALUE(infos_function_prefix);
_GENERATE_GET_VALUE(infos_file_prefix);
_GENERATE_GET_VALUE(infos_line_prefix);
_GENERATE_GET_VALUE(infos_fcolor);
_GENERATE_GET_VALUE(infos_bcolor);
_GENERATE_GET_VALUE(infos_style);
// message
_GENERATE_GET_VALUE(msg_prefix);
_GENERATE_GET_VALUE(msg_prefix_fcolor);
_GENERATE_GET_VALUE(msg_prefix_bcolor);
_GENERATE_GET_VALUE(msg_prefix_style);
_GENERATE_GET_VALUE(msg_fcolor);
_GENERATE_GET_VALUE(msg_bcolor);
_GENERATE_GET_VALUE(msg_style);
_GENERATE_GET_VALUE(msg_values_fcolor);
_GENERATE_GET_VALUE(msg_values_bcolor);
_GENERATE_GET_VALUE(msg_values_style);
_GENERATE_GET_VALUE(msg_append_newline);

template<typename... Configs>
struct GenerateConfig :
        _get_conf_name_value<Configs...>,
        // date & time
        _get_dt_date_display_value<Configs...>,
        _get_dt_time_display_value<Configs...>,
        _get_dt_prefix_value<Configs...>,
        _get_dt_suffix_value<Configs...>,
        _get_dt_delimiters_fcolor_value<Configs...>,
        _get_dt_delimiters_bcolor_value<Configs...>,
        _get_dt_delimiters_style_value<Configs...>,
        _get_dt_separator_value<Configs...>,
        _get_dt_fcolor_value<Configs...>,
        _get_dt_bcolor_value<Configs...>,
        _get_dt_style_value<Configs...>,
        // tag
        _get_tag_value<Configs...>,
        _get_tag_display_value<Configs...>,
        _get_tag_prefix_value<Configs...>,
        _get_tag_suffix_value<Configs...>,
        _get_tag_delimiters_fcolor_value<Configs...>,
        _get_tag_delimiters_bcolor_value<Configs...>,
        _get_tag_delimiters_style_value<Configs...>,
        _get_tag_fcolor_value<Configs...>,
        _get_tag_bcolor_value<Configs...>,
        _get_tag_style_value<Configs...>,
        // infos
        _get_infos_display_value<Configs...>,
        _get_infos_prefix_value<Configs...>,
        _get_infos_suffix_value<Configs...>,
        _get_infos_delimiters_fcolor_value<Configs...>,
        _get_infos_delimiters_bcolor_value<Configs...>,
        _get_infos_delimiters_style_value<Configs...>,
        _get_infos_separator_value<Configs...>,
        _get_infos_display_function_value<Configs...>,
        _get_infos_display_file_value<Configs...>,
        _get_infos_display_line_value<Configs...>,
        _get_infos_function_prefix_value<Configs...>,
        _get_infos_file_prefix_value<Configs...>,
        _get_infos_line_prefix_value<Configs...>,
        _get_infos_fcolor_value<Configs...>,
        _get_infos_bcolor_value<Configs...>,
        _get_infos_style_value<Configs...>,
        // message
        _get_msg_prefix_value<Configs...>,
        _get_msg_prefix_fcolor_value<Configs...>,
        _get_msg_prefix_bcolor_value<Configs...>,
        _get_msg_prefix_style_value<Configs...>,
        _get_msg_fcolor_value<Configs...>,
        _get_msg_bcolor_value<Configs...>,
        _get_msg_style_value<Configs...>,
        _get_msg_values_fcolor_value<Configs...>,
        _get_msg_values_bcolor_value<Configs...>,
        _get_msg_values_style_value<Configs...>,
        _get_msg_append_newline_value<Configs...> {
};

// from Loki. select a type with a flag, avoid unessessary traits
template<bool flag, typename T, typename U> struct Select              { typedef T Result; };
template<typename T, typename U>            struct Select<false, T, U> { typedef U Result; };

} // _private

struct _liog_config_name(DefaultOverride);

namespace traits {

// enable override of default config
template<typename T> struct is_default_config_overriden { static constexpr bool value = false; };

// define what type is a config so we can static_assert in a bad usage of liog
template<typename T> struct is_config                    { static constexpr bool value = false; };
template<>           struct is_config<config::t_Default> { static constexpr bool value = true;  };

template<typename T> struct select_config {
    typedef typename _private::Select<traits::is_default_config_overriden<T>::value, _liog_config_name(DefaultOverride), T>::Result result;
};

} // traits

namespace _private {

inline std::ostream& reset_font_if_necessary(std::ostream& os) {
    if (_private::current_font.m_fcolor != fcolor::original || _private::current_font.m_bcolor != bcolor::original ||
            _private::current_font.m_style != style::original) {
        os << resetfont;
    }
    return os;
}

} // _private

template<typename Config>
void tliog_message() {
    std::cout << _private::reset_font_if_necessary;
    if (Config::msg_append_newline) {
        std::cout << std::endl;
    } else {
        std::cout << std::flush;
    }
}

template<typename Config, typename T>
void tliog_message(const T& object) {
    std::cout << object;
    tliog_message<Config>();
}

template<typename Config, typename T, typename... Args>
void tliog_message(const char* format, const T& value, Args... args) {
    for (; *format != '\0'; ++format) {
        // % is escaped with \\, display it
        if (*format == '\\') {
            if (*(format + 1) != '\0') {
                if (*(format + 1) == '%') {
                    std::cout << '%';
                    format += 1;
                    continue;
                }
            }
        }
        // replace % with value
        if (*format == '%') {
            // color values option
            if (!traits::is_font_attribute<T>::value && (Config::msg_values_fcolor != fcolor::current || // test all 'current' to avoid the temporary save
                    Config::msg_values_bcolor != bcolor::current || Config::msg_values_style != style::current)) {
                std::cout << (_private::temporary_font = { Config::msg_values_fcolor, Config::msg_values_bcolor, Config::msg_values_style });
                std::cout << value;
                std::cout << (_private::temporary_font = { Config::msg_fcolor, Config::msg_bcolor, Config::msg_style });
            } else {
                std::cout << value;
            }
            tliog_message<Config>(format + 1, args...); // recursive call
            return;
        }
        // print current format letter
        std::cout << *format;
    }
}

template<typename Config>
void tliog_apply_config(const char* /*time*/, const char* /*date*/, const char* file, int line, const char* function) {
    //    static_assert(Config::tag != nullptr && !Config::tag_display, "Undefined tag! Use liog_set(tag) = \"your_tag\".");

    // date & time
    if (Config::dt_date_display || Config::dt_time_display) {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        if (Config::dt_prefix != nullptr) {
            std::cout << (_private::temporary_font = { Config::dt_delimiters_fcolor, Config::dt_delimiters_bcolor, Config::dt_delimiters_style });
            std::cout << Config::dt_prefix;
        }
        std::cout << (_private::temporary_font = { Config::dt_fcolor, Config::dt_bcolor, Config::dt_style });
        if (Config::dt_date_display) {
            std::cout << std::put_time(std::localtime(&now_c), "%Y.%m.%d");
        }
        if (Config::dt_time_display) {
            if (Config::dt_date_display) {
                std::cout << Config::dt_separator;
            }
            std::cout << std::put_time(std::localtime(&now_c), "%H:%M:%S");
        }
        if (Config::dt_suffix != nullptr) {
            std::cout << (_private::temporary_font = { Config::dt_delimiters_fcolor, Config::dt_delimiters_bcolor, Config::dt_delimiters_style });
            std::cout << Config::dt_suffix;
        }
    }

    // tag
    if (Config::tag_display) {
        if (Config::tag_prefix != nullptr) {
            std::cout << (_private::temporary_font = { Config::tag_delimiters_fcolor, Config::tag_delimiters_bcolor, Config::tag_delimiters_style });
            std::cout << Config::tag_prefix;
        }
        if (Config::tag != nullptr) {
            std::cout <<(_private:: temporary_font = { Config::tag_fcolor, Config::tag_bcolor, Config::tag_style });
            std::cout << Config::tag;
        }
        if (Config::tag_suffix != nullptr) {
            std::cout << (_private::temporary_font = { Config::tag_delimiters_fcolor, Config::tag_delimiters_bcolor, Config::tag_delimiters_style });
            std::cout << Config::tag_suffix;
        }
    }

    // infos
    if (Config::infos_display) {
        if (Config::infos_prefix != nullptr) {
            std::cout << (_private::temporary_font = { Config::infos_delimiters_fcolor, Config::infos_delimiters_bcolor, Config::infos_delimiters_style });
            std::cout << Config::infos_prefix;
        }
        if (Config::infos_display_function || Config::infos_display_file || Config::infos_display_line) {
            std::cout << (_private::temporary_font = { Config::infos_fcolor, Config::infos_bcolor, Config::infos_style });
            if (Config::infos_display_function) {
                std::cout << function;
            }
            if (Config::infos_display_file) {
                if (Config::infos_display_function) {
                    std::cout << Config::infos_separator;
                }
                std::cout << Config::infos_file_prefix << file;
            }
            if (Config::infos_display_line) {
                if (Config::infos_display_function || Config::infos_display_file) {
                    std::cout << Config::infos_separator;
                }
                std::cout << Config::infos_line_prefix << line;
            }
        }
        if (Config::infos_suffix != nullptr) {
            std::cout << (_private::temporary_font = { Config::infos_delimiters_fcolor, Config::infos_delimiters_bcolor, Config::infos_delimiters_style });
            std::cout << Config::infos_suffix;
        }
    }

    // message
    if (Config::msg_prefix != nullptr) {
        std::cout << (_private::temporary_font = { Config::msg_prefix_fcolor, Config::msg_prefix_bcolor, Config::msg_prefix_style });
        std::cout << Config::msg_prefix;
    }
    // TODO: don't set the color if the message is empty -> can be done in tliog_message now since we have the Config
    std::cout << (_private::temporary_font = { Config::msg_fcolor, Config::msg_bcolor, Config::msg_style });
}

template<typename Config>
inline void tliog_entry_point(const char* time, const char* date, const char* file, int line, const char* function) {
    static_assert(traits::is_config<Config>::value, "Unknown log config!");
    typedef typename traits::select_config<Config>::result SelectedConfig;
    tliog_apply_config<SelectedConfig>(time, date, file, line, function);
    tliog_message<SelectedConfig>();
}

template<typename Config, typename T>
inline void tliog_entry_point(const char* time, const char* date,  const char* file, int line, const char* function,
                              const T& value) {
    static_assert(traits::is_config<Config>::value, "Unknown log config!");
    typedef typename traits::select_config<Config>::result SelectedConfig;
    tliog_apply_config<SelectedConfig>(time, date, file, line, function);
    tliog_message<SelectedConfig>(value);
}

template<typename Config, typename T, typename... Args>
inline void tliog_entry_point(const char* time, const char* date, const char* file, int line, const char* function,
                              const char* format, const T& value, Args... args) {    
    static_assert(traits::is_config<Config>::value, "Unknown log config!");
    typedef typename traits::select_config<Config>::result SelectedConfig;
    tliog_apply_config<SelectedConfig>(time, date, file, line, function);
    tliog_message<SelectedConfig>(format, value, args...);
}

} // nliog

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // LIOG
