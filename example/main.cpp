#include "liogcfg.hpp"

// wanna add support for custom types?
// implement << operator

int main(int, char**) {
    liog(); // use the default configuration
    liog(MSG);
    liog(MSG, "Hello");
    std::string str("ID");
    liog(MSG, "Variables like % or % can have a different color than the message!", 5, str);
    liog(MSG, "%"); // no need to escape the sign because there is only one param, this is not considered as a format string
    liog(MSG, "display percent sign: %%", 50); // no need to escape the sign because there is no params after 50
    liog(MSG, "%yellow!% percent test \\%, %cyan%!", // escape the % with double backslash
        fcolor::yellow, resetfont, fcolor::cyan, resetfont);
    // not optimized, it applies the color code 3 times
    liog(MSG, "%%%underlined, blue foreground, red background", style::underlined, fcolor::blue, bcolor::red);
    // optimized code, the color code is applied only 1 time
    liog(MSG, "%underlined, blue foreground, red background", font(fcolor::blue, bcolor::red, style::underlined));
    liog(MSG, "%%red on white% / %green", fcolor::red, bcolor::white, resetfont, fcolor::green);
    liog(MSG, "%red + %underlined + %current (does nothing) + %font(green, blue, current) + %current + %bcolor white + "
             "%black + %not underlined",
        fcolor::red, style::underlined, fcolor::current, font(fcolor::green, bcolor::blue, style::current),
        fcolor::current, bcolor::white, fcolor::black, style::none);
    liog(RAW, "No new line"); liog(RAW, " at the %", "end"); liog(RAW, " of this message.");
    liog(RAW, "\n"); // add one
    liog(WARNING, "Where is wally?");
    liog(ERROR, "Cannot find %", "wally");
    return 0;
}
