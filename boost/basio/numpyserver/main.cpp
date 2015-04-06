/*
Test with eg::

        udpserver-test 
        UDP_PORT=13 udp.py hello


        npysend.sh --tag 1  # requires zmq- ; zmq-broker 

*/

#include "numpydelegate.hpp"
#include "numpyserver.hpp"

int main()
{
    numpydelegate nde ;  // example numpydelegate
    numpyserver<numpydelegate> srv(&nde, 8080, "tcp://127.0.0.1:5002");
    nde.setServer(&srv); // needed for replying to posts from delegate calls
    
    for(unsigned int i=0 ; i < 20 ; ++i )
    {
        srv.poll();
        srv.sleep(1);
        //std::string msg = std::to_string(i) ;
        //srv.send( msg );
    }

    return 0;
}

