import 'package:andon/esp.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
// import 'package:andon/memori.dart';

class Update extends StatefulWidget {
  @override
  _UpdateState createState() => _UpdateState();
}

class _UpdateState extends State<Update> {
  // Memori _eprom = Memori();
  Esp esp = Esp(4210);
  bool play = true;
  @override
  void initState() {
    super.initState();
  }

  // _saveData(String key, String value) async {
  //   await _eprom.setString(key, value);
  // }

  TextEditingController _textFieldController = TextEditingController();
  _displayDialog(
      BuildContext context, String judul, String hint, int maxLengt) async {
    return showDialog(
        context: context,
        builder: (context) {
          return AlertDialog(
            title: Text(judul),
            content: TextField(
              controller: _textFieldController,
              decoration: InputDecoration(hintText: hint),
              keyboardType: TextInputType.number,
              inputFormatters: <TextInputFormatter>[
                WhitelistingTextInputFormatter.digitsOnly,
              ],
              maxLength: maxLengt,
            ),
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
                  String cmd;
                  if (judul == 'Set Aktual') {
                    cmd = '*009=';
                  }
                  if (judul == 'Set Target') {
                    cmd = '*010=';
                  }
                  if (judul == 'Setting Timer') {
                    cmd = '*013=';
                    var len = _textFieldController.text.length;
                    while (len < 7 && len > 0) {
                      _textFieldController.text =
                          '0' + _textFieldController.text;
                      len++;
                    }
                  }

                  var panjang = _textFieldController.text.length;
                  // print('panjang = ' + panjang.toString());
                  if (panjang > 0) {
                    while (panjang < 4) {
                      _textFieldController.text =
                          '0' + _textFieldController.text;
                      panjang++;
                    }
                    esp.send(cmd + _textFieldController.text + '#');
                    // print(cmd + _textFieldController.text + '#');
                  }
                  Navigator.of(context).pop();
                },
              )
            ],
          );
        });
  }

  _displayReset(BuildContext context, String judul, String masage,
      VoidCallback onTap) async {
    return showDialog(
        context: context,
        builder: (context) {
          return AlertDialog(
            title: Text(judul),
            content: Text("Reset data " + masage),
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
                  onTap();
                  Navigator.of(context).pop();
                },
              )
            ],
          );
        });
  }

  void resetTarget() {
    esp.send('*012=0#');
    // print("resetTarget");
    // esp.send("value");
  }

  void resetActual() {
    esp.send('*011=0#');
    // print("resetActual");
    // esp.send("value");
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      // color: Colors.white,
      padding: EdgeInsets.all(9.0),
      child: GridView.count(
        crossAxisCount: 2,
        children: <Widget>[
          MyMenu(
              nama: "Set Aktual",
              gambar: "assets/launcher_icon.png",
              warna: Colors.green,
              index: 0,
              onTap: () {
                _textFieldController.clear();
                _displayDialog(
                    context, "Set Aktual", "Masukan jumlah aktual", 4);
              }),
          MyMenu(
              nama: "Set Target",
              gambar: "assets/launcher_icon.png",
              warna: Colors.amber,
              index: 1,
              onTap: () {
                _textFieldController.clear();
                _displayDialog(
                    context, "Set Target", "Masukan jumlah target", 4);
              }
              // onTap: () => _displayDialog,
              ),
          MyMenu(
            nama: "Reset Aktual",
            gambar: "assets/reset.png",
            warna: Colors.purple,
            index: 2,
            onTap: () =>
                _displayReset(context, "Reset Aktual", "aktual ", resetActual),
          ),
          MyMenu(
            nama: "Reset Target",
            gambar: "assets/reset.png",
            warna: Colors.lime,
            index: 3,
            onTap: () =>
                _displayReset(context, "Reset Target", " target ", resetTarget),
          ),
          MyMenu(
              nama: "Set Timer",
              gambar: "assets/stopwatch.png",
              warna: Colors.amber,
              index: 4,
              onTap: () {
                _textFieldController.clear();
                _displayDialog(context, "Setting Timer", "milli second", 7);
              }
              // onTap: () => _displayDialog,
              ),
          MyMenu(
              nama: play == true ? "Play" : "Pause",
              gambar: play == true ? "assets/play.png" : "assets/pause.png",
              warna: Colors.red,
              index: 5,
              onTap: () {
                if (play == true) {
                  esp.send('*014=0#');
                  setState(() {
                    play = false;
                  });
                } else {
                  esp.send('*014=1#');
                  setState(() {
                    play = true;
                  });
                }
              }),
        ],
      ),
    );
  }
}

class MyMenu extends StatelessWidget {
  const MyMenu({this.nama, this.gambar, this.warna, this.index, this.onTap});
  final String nama;
  final String gambar;
  final MaterialColor warna;
  final int index;
  final VoidCallback onTap;

  @override
  Widget build(BuildContext context) {
    return Container(
        child: Card(
      margin: EdgeInsets.all(9.0),
      child: InkWell(
        onTap: () => onTap(),
        splashColor: Colors.yellow,
        child: Column(
          // mainAxisSize: MainAxisSize.min,
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: <Widget>[
            Container(
              child: Image.asset(
                gambar,
                width: MediaQuery.of(context).size.width * 0.27,
                height: MediaQuery.of(context).size.width * 0.27,
                // fit: BoxFit.cover,
              ),
            ),
            // Icon(
            //   gambar,
            //   size: 90,
            //   color: warna,
            // ),
            Text(
              nama,
              style: TextStyle(fontSize: 20),
            )
          ],
        ),
      ),
    ));
  }
}
