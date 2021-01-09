import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:settings_ui/settings_ui.dart';
import 'dart:async';
import 'package:andon/memori.dart';
import 'package:andon/esp.dart';

class Setting extends StatefulWidget {
  @override
  _SettingState createState() => _SettingState();
}

class _SettingState extends State<Setting> {
  List<String> mytext = new List(70);
  Memori _eprom = new Memori();
  TimeOfDay selectedTime = TimeOfDay.now();
  Esp _nodemcu = Esp(4210); //udp ke nodemcu
  Timer _timer;
  int _start = 2;
  TextEditingController _textFieldController = TextEditingController();

  @override
  void dispose() {
    _timer.cancel();
    super.dispose();
  }

  @override
  void initState() {
    // _textFieldController.addListener(filter);
    for (var i = 0; i < 70; i++) {
      if (((i + 1) % 9) == 0) {
        // await _eprom.setString('parameter' + i.toString(), '0000');
        mytext[i] = "0000";
      } else {
        // await _eprom.setString('parameter' + i.toString(), '00:00');
        mytext[i] = "00:00";
      }
    }
    _loaddata();
    super.initState();
  }

  // filter() {print

  // }

  void _kirim() {
    const space = const Duration(milliseconds: 200);
    _timer = new Timer.periodic(
      space,
      (Timer timer) => setState(
        () {
          if (_start > 8) {
            _start = 2;
            timer.cancel();
          } else {
            String send;
            send = mytext.join();
            send = send.replaceAll(':', '');
            if (_start == 2) {
              _nodemcu.send('*002=' + send.substring(0, 36) + '#');
            }
            if (_start == 3) {
              _nodemcu.send('*003=' + send.substring(36, 72) + '#');
            }
            if (_start == 4) {
              _nodemcu.send('*004=' + send.substring(72, 108) + '#');
            }
            if (_start == 5) {
              _nodemcu.send('*005=' + send.substring(108, 144) + '#');
            }
            if (_start == 6) {
              _nodemcu.send('*006=' + send.substring(144, 180) + '#');
            }
            if (_start == 7) {
              _nodemcu.send('*007=' + send.substring(180, 216) + '#');
            }
            if (_start == 8) {
              _nodemcu.send('*008=' + send.substring(216, 252) + '#');
            }
            _start += 1;
          }
        },
      ),
    );
  }

  _loaddata() async {
    for (var i = 0; i < 70; i++) {
      String _eValue = await _eprom.getString('parameter' + i.toString());
      if (_eValue.length < 4) {
        if (((i + 1) % 9) == 0) {
          await _eprom.setString('parameter' + i.toString(), '0000');
        } else {
          await _eprom.setString('parameter' + i.toString(), '00:00');
        }
      } else {
        setState(() {
          mytext[i] = _eValue;
        });
      }
    }
  }

  _saveData(String key, String value) async {
    await _eprom.setString(key, value);
  }

  Future<Null> _selectedTime(BuildContext context, int index) async {
    final TimeOfDay pickedTime = await showTimePicker(
      context: context,
      initialTime: selectedTime,
    );

    if (pickedTime != null) {
      //&& pickedTime != selectedTime
      // print("${selectedTime.format(context)}");
      setState(() {
        selectedTime = pickedTime;
        mytext[index] = "${selectedTime.format(context)}";
        _saveData(
            'parameter' + index.toString(), "${selectedTime.format(context)}");
      });
    }
  }

  Widget _setJam(String judul, int index) {
    return SettingsTile(
      title: judul,
      // subtitle: subJudul,
      // leading: Icon(
      //   icon,
      //   size: 35.0,
      //   color: Colors.green,
      // ),
      trailing: Text(mytext[index]),
      onTap: () {
        // print(mytext[index]);
        var sTime = mytext[index].split(':');
        selectedTime =
            TimeOfDay(hour: int.parse(sTime[0]), minute: int.parse(sTime[1]));
        _selectedTime(context, index);
      },
    );
  }

  Widget _setPlan(String judul, int index) {
    // _saveData(String key, String value)
    // await _eprom.setString('parameter' + i.toString(), '00:00');
    return SettingsTile(
      title: judul,
      trailing: Text(mytext[index]),
      onTap: () {
        _textFieldController.text = mytext[index];
        _displayDialog(context, "Plan Setting", "Masukan Jumlah Plan", index);
      },
    );
  }

  _displayDialog(
      BuildContext context, String judul, String hint, int index) async {
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
                WhitelistingTextInputFormatter.digitsOnly
              ],
              maxLength: 4,
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
                  var panjang = _textFieldController.text.length;
                  while (panjang < 4) {
                    _textFieldController.text = '0' + _textFieldController.text;
                    panjang++;
                  }
                  setState(() {
                    mytext[index] = _textFieldController.text;
                  });
                  _saveData('parameter' + index.toString(),
                      _textFieldController.text);
                  Navigator.of(context).pop();
                },
              )
            ],
          );
        });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: SettingsList(
        // backgroundColor: Colors.orange,
        // lightBackgroundColor: Colors.green,
        // darkBackgroundColor: Colors.yellow,
        sections: [
          SettingsSection(
            title: 'Senin',
            tiles: [
              _setJam("Waktu mulai bekerja", 0),
              _setJam("Waktu mulai istirahat 1", 1),
              _setJam("Waktu selesai istirahat 1", 2),
              _setJam("Waktu mulai istirahat 2", 3),
              _setJam("Waktu selesai istirahat 2", 4),
              _setJam("Waktu mulai istirahat 3", 5),
              _setJam("Waktu selesai istirahat 3", 6),
              _setJam("Waktu selesai bekerja", 7),
              _setPlan("Setting jumlah Plan", 8),
            ],
          ),
          SettingsSection(
            title: 'Selasa',
            tiles: [
              _setJam("Waktu mulai bekerja", 9),
              _setJam("Waktu mulai istirahat 1", 10),
              _setJam("Waktu selesai istirahat 1", 11),
              _setJam("Waktu mulai istirahat 2", 12),
              _setJam("Waktu selesai istirahat 2", 13),
              _setJam("Waktu mulai istirahat 3", 14),
              _setJam("Waktu selesai istirahat 3", 15),
              _setJam("Waktu selesai bekerja", 16),
              _setPlan("Setting jumlah Plan", 17),
            ],
          ),
          SettingsSection(
            title: 'Rabu',
            tiles: [
              _setJam("Waktu mulai bekerja", 18),
              _setJam("Waktu mulai istirahat 1", 19),
              _setJam("Waktu selesai istirahat 1", 20),
              _setJam("Waktu mulai istirahat 2", 21),
              _setJam("Waktu selesai istirahat 2", 22),
              _setJam("Waktu mulai istirahat 3", 23),
              _setJam("Waktu selesai istirahat 3", 24),
              _setJam("Waktu selesai bekerja", 25),
              _setPlan("Setting jumlah Plan", 26),
            ],
          ),
          SettingsSection(
            title: 'Kamis',
            tiles: [
              _setJam("Waktu mulai bekerja", 27),
              _setJam("Waktu mulai istirahat 1", 28),
              _setJam("Waktu selesai istirahat 1", 29),
              _setJam("Waktu mulai istirahat 2", 30),
              _setJam("Waktu selesai istirahat 2", 31),
              _setJam("Waktu mulai istirahat 3", 32),
              _setJam("Waktu selesai istirahat 3", 33),
              _setJam("Waktu selesai bekerja", 34),
              _setPlan("Setting jumlah Plan", 35),
            ],
          ),
          SettingsSection(
            title: 'Jumat',
            tiles: [
              _setJam("Waktu mulai bekerja", 36),
              _setJam("Waktu mulai istirahat 1", 37),
              _setJam("Waktu selesai istirahat 1", 38),
              _setJam("Waktu mulai istirahat 2", 39),
              _setJam("Waktu selesai istirahat 2", 40),
              _setJam("Waktu mulai istirahat 3", 41),
              _setJam("Waktu selesai istirahat 3", 42),
              _setJam("Waktu selesai bekerja", 43),
              _setPlan("Setting jumlah Plan", 44),
            ],
          ),
          SettingsSection(
            title: 'Sabtu',
            tiles: [
              _setJam("Waktu mulai bekerja", 45),
              _setJam("Waktu mulai istirahat 1", 46),
              _setJam("Waktu selesai istirahat 1", 47),
              _setJam("Waktu mulai istirahat 2", 48),
              _setJam("Waktu selesai istirahat 2", 49),
              _setJam("Waktu mulai istirahat 3", 50),
              _setJam("Waktu selesai istirahat 3", 51),
              _setJam("Waktu selesai bekerja", 52),
              _setPlan("Setting jumlah Plan", 53),
            ],
          ),
          SettingsSection(
            title: 'Minggu',
            tiles: [
              _setJam("Waktu mulai bekerja", 54),
              _setJam("Waktu mulai istirahat 1", 55),
              _setJam("Waktu selesai istirahat 1", 56),
              _setJam("Waktu mulai istirahat 2", 57),
              _setJam("Waktu selesai istirahat 2", 58),
              _setJam("Waktu mulai istirahat 3", 59),
              _setJam("Waktu selesai istirahat 3", 60),
              _setJam("Waktu selesai bekerja", 61),
              _setPlan("Setting jumlah Plan", 62),
            ],
          ),
          // SettingsSection(
          //   title: 'Common',
          //   // titleTextStyle: TextStyle(fontSize: 30),
          //   tiles: [
          //     SettingsTile(
          //       title: 'Language',
          //       subtitle: 'English',
          //       leading: Icon(Icons.language),
          //       onTap: () {
          //         // Navigator.of(context).push(MaterialPageRoute(
          //         //     builder: (BuildContext context) => LanguagesScreen()));
          //       },
          //     ),
          //     SettingsTile(
          //       title: 'Environment',
          //       subtitle: 'Production',
          //       leading: Icon(Icons.cloud_queue),
          //       onTap: () => print('e'),
          //     ),
          //   ],
          // ),
          // SettingsSection(
          //   title: 'Account',
          //   tiles: [
          //     SettingsTile(title: 'Phone number', leading: Icon(Icons.phone)),
          //     SettingsTile(title: 'Email', leading: Icon(Icons.email)),
          //     SettingsTile(title: 'Sign out', leading: Icon(Icons.exit_to_app)),
          //   ],
          // ),
          // SettingsSection(
          //   title: 'Security',
          //   tiles: [
          //     SettingsTile.switchTile(
          //       title: 'Lock app in background',
          //       leading: Icon(Icons.phonelink_lock),
          //       switchValue: lockInBackground,
          //       onToggle: (bool value) {
          //         setState(() {
          //           lockInBackground = value;
          //           notificationsEnabled = value;
          //         });
          //       },
          //     ),
          //     SettingsTile.switchTile(
          //         title: 'Use fingerprint',
          //         leading: Icon(Icons.fingerprint),
          //         onToggle: (bool value) {},
          //         switchValue: false),
          //     SettingsTile.switchTile(
          //       title: 'Change password',
          //       leading: Icon(Icons.lock),
          //       switchValue: true,
          //       onToggle: (bool value) {},
          //     ),
          //     SettingsTile.switchTile(
          //       title: 'Enable Notifications',
          //       enabled: notificationsEnabled,
          //       leading: Icon(Icons.notifications_active),
          //       switchValue: true,
          //       onToggle: (value) {},
          //     ),
          //   ],
          // ),
          // SettingsSection(
          //   title: 'Misc',
          //   tiles: [
          //     SettingsTile(
          //         title: 'Terms of Service', leading: Icon(Icons.description)),
          //     SettingsTile(
          //         title: 'Open source licenses',
          //         leading: Icon(Icons.collections_bookmark)),
          //   ],
          // ),
          CustomSection(
            child: Column(
              children: [
                Padding(
                  padding: const EdgeInsets.only(top: 22, bottom: 8),
                  child: Image.asset(
                    'assets/settings.png',
                    height: 50,
                    width: 50,
                    color: Color(0xFF777777),
                  ),
                ),
                Text(
                  'Counter Produksi',
                  style: TextStyle(color: Color(0xFF777777)),
                ),
                Text(
                  'Version: 1.0.0 ',
                  style: TextStyle(color: Color(0xFF777777)),
                ),
              ],
            ),
          ),
        ],
        // ),
      ),
      floatingActionButton: FloatingActionButton(
        child: Icon(Icons.file_upload),
        onPressed: () {
          _kirim();
        },
      ),
    );
  }
}
