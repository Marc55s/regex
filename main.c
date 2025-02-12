#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool Do_Match_Star(char c, char *regex, int reidx, char *text, int textidx);
bool Do_Match(char *regex, int reidx, char *text, int textidx);
bool Match(char *regex, char *text);
static bool test(char *regex, char *text, bool expected);

static bool test(char *regex, char *text, bool expected) {
    bool actual = Match(regex, text);
    if (actual != expected) {
        printf("Match der Regex '{%s}' in Text '{%s}' failed!\n",regex,text);
    } else {
        printf("Match der Regex '{%s}' in Text '{%s}' succeeded!\n",regex,text);
    }

    return actual == expected;
}

bool Match(char *regex, char *text) {
    if (strlen(regex) > 0 && regex[0] == '^')
        return Do_Match(regex, 1, text, 0);

    for (int i = 0; i <= strlen(text); i++) // Regex starting somewhere in text
    {                                       // <= for Regex=$
        // try match from text [i], when found: return true
        if (Do_Match(regex, 0, text, i)) {
            return true;
        }
    }
    return false; // not contained
}

bool Do_Match(char *regex, int reidx, char *text, int textidx) {
    if (reidx >= strlen(regex)) // regex completed
        return true;

    if (regex[reidx] == '$' && reidx == strlen(regex) - 1 &&
        textidx >=
        strlen(text)) { // $ in Regex and regex completed and text completed
        return true;
    }

    // new for *
    if (reidx != strlen(regex) - 1 && regex[reidx + 1] == '*')
        return Do_Match_Star(regex[reidx], regex, reidx + 2, text, textidx);

    if (textidx >= strlen(text)) { // Text completed, Regex not completed
        return false;
    }

    if ((regex[reidx] == '.') ||
        (regex[reidx] == text[textidx]))                    // char similar or .
        return Do_Match(regex, reidx + 1, text, textidx + 1); // continue

    return false; // e.g. $ found and text not completed
}
bool Do_Match_Star(char c, char *regex, int reidx, char *text, int textidx) {
    // TODO

    return false;
}

int main(void) {
    // Test cases
    test("llo", "Hello world!", true);
    test("d!", "Hello world!", true);
    test("H", "Hello world!", true);
    test("", "Hello world!", true);

    test("123", "Hello world!", false);
    test("Hello world!abc", "Hello world!", false);

    test("l.o", "Hello world!", true);
    test("....", "Hello world!", true);
    test("Hello.world!", "Hello world!", true);

    test("!.", "Hello world!", false);
    test(".H", "Hello world!", false);
    test("l.a", "Hello world!", false);

    test("^Hello", "Hello world!", true);
    test("^H", "Hello world!", true);
    test("^Hello world!", "Hello world!", true);
    test("^", "Hello world!", true);

    test("^world", "Hello world!", false);
    test("^h", "Hello world!", false);

    test("$", "Hello world!", true);
    test("!$", "Hello world!", true);
    test("world!$", "Hello world!", true);
    test("^Hello world!$", "Hello world!", true);

    test("Hello$", "Hello world!", false);

    test("Hel*o", "Hello world!", true);
    test("worl*d", "Hello world!", true);
    test("^Hello *world!$", "Hello world!", true);
    test("^.*$", "Hello world!", true);
    test("l.*d", "Hello world!", true);
    test("wo_*rld", "Hello world!", true);

    test("ex*o", "Hello world!", false);
    test("x.*", "Hello world!", false);

    return 0;
}
