with open('y.tab.h', 'r+') as fd:
    content = fd.read();
    fd.seek(0, 0);
    fd.write('#include "ast.h"\n'+content);
