
#include <memory>

#include "operations/dev/echo_operation.h"
#include "webserver/webserver.h"

int main(int argc, char* argv[])
{
    webserver server(8080);
    server.add_operation(std::make_shared<echo_operation>());

    while (true)
    {
        
    }
    
    return 0;
}
