import 'package:udp/udp.dart';

class Esp {
  int myport;
  Esp(int port) {
    this.myport = port;
  }
  void send(String value) async {
    // creates a UDP instance and binds it to the first available network
    // interface on port 65000.
    var sender = await UDP.bind(Endpoint.any(port: Port(65000)));
    // send a simple string to a broadcast endpoint on port 65001.
    var dataLength = await sender.send(
        value.codeUnits, Endpoint.broadcast(port: Port(myport)));

    // print("$dataLength bytes sent.");

    // creates a new UDP instance and binds it to the local address and the port
    // 65002.
    // var receiver = await UDP.bind(Endpoint.loopback(port: Port(65002)));

    // receiving listening
    await sender.listen((datagram) {
      var str = String.fromCharCodes(datagram.data);
      // print(str);
    }, timeout: Duration(seconds: 20));

    // close the UDP instances and their sockets.
    sender.close();
  }
}
