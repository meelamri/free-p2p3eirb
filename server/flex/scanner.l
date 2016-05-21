%{
#include "./flex/lex.yy.h"
%}
%%
\[                    return OPEN_HOOK;
\]                    return CLOSED_HOOK;
\"                    return QUOTE;
[=<>]                 return OPERATOR;
"filename"            return FILE_NAME;
"filesize"            return FILE_SIZE;
"announce"            return ANNOUNCE;
"look"                return LOOK;
"getfile"             return GETFILE;
"update"              return UPDATE;
"listen"              return LISTEN;
"seed"                return SEED;
"leech"               return LEECH;
[a-zA-Z][\.\_a-zA-Z0-9]*  return IDENTIFIER;
[0-9][0-9]*           return INTEGER;
[0-9]+[0-9a-zA-Z]*          return KEY;
[ \t\n]+               ;
.                     { ECHO; }

%%

int yywrap(void)
{
 return 1;
}
