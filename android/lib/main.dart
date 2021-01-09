import 'dart:io';
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'pages/setting.dart';
import 'pages/update.dart';
import 'package:grafpix/icons.dart';
import 'package:app_settings/app_settings.dart';
import 'esp.dart';
// import 'memori.dart';
import 'package:intl/intl.dart';

void enablePlatformOverrideForDesktop() {
  if (!kIsWeb && (Platform.isMacOS || Platform.isWindows || Platform.isLinux)) {
    debugDefaultTargetPlatformOverride = TargetPlatform.fuchsia;
  }
}

void main() {
  enablePlatformOverrideForDesktop();
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Counter Produksi',
      theme: ThemeData(
        primarySwatch: Colors.pink,
      ),
      debugShowCheckedModeBanner: false,
      home: RootPage(),
      // initialRoute: '/',
      // routes: {
      //   '/': (context) => RootPage(),
      //   // When navigating to the "/second" route, build the SecondScreen widget.
      //   '/customData': (context) => CustomDataTable(),
      // },
    );
  }
}

class RootPage extends StatelessWidget {
  final Esp nodemcu = Esp(4210); //udp ke nodemcu
  void _setTanggal() {
    DateTime now = DateTime.now();
    String formattedDate = DateFormat('kkmmssddMMyyyy').format(now);
    //kk = jam
    //mm = menit
    //ss = detik
    //MM = bulan
    //dd = tanggalflutter clean
    //yyyy = tahun
    nodemcu.send('*001=' + formattedDate + '#');
  }

  _displayDialog(BuildContext context) async {
    return showDialog(
        context: context,
        builder: (context) {
          return AlertDialog(
            title: Text("Sinkronkan waktu"),
            content:
                Text("Update waktu pada display dengan waktu pada android"),
            actions: <Widget>[
              new FlatButton(
                child: new Text('CANCEL'),
                onPressed: () {
                  Navigator.of(context).pop();
                },
              ),
              new FlatButton(
                child: new Text('OK'),
                onPressed: () {
                  _setTanggal();
                  Navigator.of(context).pop();
                },
              )
            ],
          );
        });
  }

  @override
  Widget build(BuildContext context) {
    return DefaultTabController(
      length: 2,
      child: Scaffold(
        appBar: AppBar(
          backgroundColor: Colors.brown,
          title: Text("Counter Produksi"),
          actions: [
            IconButton(
                icon: Icon(
                  PixIcon.pix_alarm,
                  color: Colors.blue,
                  size: 25,
                ),
                onPressed: () {
                  _displayDialog(context);
                  // print("object");
                }),
            IconButton(
                icon: Icon(
                  PixIcon.fa_wifi,
                  color: Colors.green,
                  size: 25,
                ),
                onPressed: () {
                  AppSettings.openWIFISettings();
                  print("object");
                }),
          ],
          bottom: TabBar(
            tabs: <Widget>[
              Tab(
                text: "Update",
              ),
              Tab(
                text: "Pengaturan",
              ),
            ],
          ),
        ),
        body: TabBarView(
          children: <Widget>[
            Update(),
            Setting(),
          ],
        ),
      ),
    );
  }
}
