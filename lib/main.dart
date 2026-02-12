import 'dart:async';
import 'dart:typed_data';

import 'package:flutter/material.dart';
import 'package:usb_serial/transaction.dart';
import 'package:usb_serial/usb_serial.dart';

void main() => runApp(MyApp());

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  UsbPort? _port;
  String _status = "Idle";
  String _errorMessage = "";
  String _weightValue = "0.00";
  List<Widget> _ports = [];
  List<Widget> _serialData = [];

  StreamSubscription<String>? _subscription;
  Transaction<String>? _transaction;
  UsbDevice? _device;
  Timer? _sendTimer;

  TextEditingController _textController = TextEditingController();

  Future<bool> _connectTo(device) async {
    _serialData.clear();

    if (_subscription != null) {
      _subscription!.cancel();
      _subscription = null;
    }

    if (_transaction != null) {
      _transaction!.dispose();
      _transaction = null;
    }

    if (_port != null) {
      _port!.close();
      _port = null;
    }

    if(_sendTimer != null){
      _sendTimer!.cancel();
      _sendTimer = null;
    }

    if (device == null) {
      _device = null;
      setState(() {
        _status = "Disconnected";
      });
      return true;
    }

    _port = await device.create();
    if (await (_port!.open()) != true) {
      setState(() {
        _status = "Failed to open port";
      });
      return false;
    }
    _device = device;

    await _port!.setDTR(true);
    await _port!.setRTS(true);
    await _port!.setPortParameters(115200, UsbPort.DATABITS_8, UsbPort.STOPBITS_1, UsbPort.PARITY_NONE);

    _transaction = Transaction.stringTerminated(_port!.inputStream as Stream<Uint8List>, Uint8List.fromList([13, 10]));

    _subscription = _transaction!.stream.listen((String line) {
      setState(() {
        _serialData.add(Text(line));
        if (_serialData.length > 20) {
          _serialData.removeAt(0);
        }

        if(line.contains("RAW_DATA")){
          try {
            List<String> parts = line.split(" ");
            if (parts.length >= 3) {
              String weightStr = parts[2]; // Lấy phần tử thứ 3
              _weightValue = weightStr;
              print("Updated weight: $_weightValue");
            }
          } catch (e) {
            print("Error parsing weight: $e");
          }
        }
      });
    });

    setState(() {
      _status = "Connected";
    });

    _startSendingData();
    return true;
  }
  void _startSendingData() {
    _sendTimer = Timer.periodic(Duration(milliseconds: 100), (timer) async {
      if (_port != null) {
        String data = "RAW_DATA\r\n";
        await _port!.write(Uint8List.fromList(data.codeUnits));
      }
    });
  }
  void _getPorts() async {
    _ports = [];
    List<UsbDevice> devices = await UsbSerial.listDevices();
    if (!devices.contains(_device)) {
      _connectTo(null);
    }
    print(devices);

    devices.forEach((device) {
      _ports.add(ListTile(
          leading: Icon(Icons.usb),
          title: Text(device.productName!),
          subtitle: Text(device.manufacturerName!),
          trailing: ElevatedButton(
            child: Text(_device == device ? "Disconnect" : "Connect"),
            onPressed: () {
              _connectTo(_device == device ? null : device).then((res) {
                _getPorts();
              });
            },
          )));
    });

    setState(() {
      print(_ports);
    });
  }

  @override
  void initState() {
    super.initState();

    UsbSerial.usbEventStream!.listen((UsbEvent event) {
      _getPorts();
    });

    _getPorts();
  }

  @override
  void dispose() {
    super.dispose();
    _connectTo(null);
  }

  Future<void> _handleConnect() async {
    if (_status == "Connected") {
      await _connectTo(null);
      _getPorts();
      return;
    }
    List<UsbDevice> devices = await UsbSerial.listDevices();

    if (devices.isEmpty) {
      setState(() {
        _errorMessage = "No device connected";
      });
      return;
    }

    devices.forEach((device) async {
      if (_device == null) {
        bool result = await _connectTo(device);
        if (result) {
          setState(() {
            _errorMessage = "Connection successfully";
          });
        } else {
          setState(() {
            _errorMessage = "Connection failed";
          });
        }
        _getPorts();
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
        debugShowCheckedModeBanner: false,
        home: HomeScreen(
          ports: _ports,
          port: _port,
          status: _status,
          serialData: _serialData,
          textController: _textController,
          errorMessage: _errorMessage,
          weightValue: _weightValue,
          onSend: () async{
            if(_port == null) {
              return;
            }

          String data = _textController.text + "\r\n";
          await _port!.write(Uint8List.fromList(data.codeUnits));
          _textController.text = "";
          },
          onConnect: _handleConnect,
        )
    );
  }
}
class HomeScreen extends StatelessWidget {
  final List<Widget> ports;
  final String status;
  final String errorMessage;
  final String weightValue;
  final UsbPort? port;
  final List<Widget> serialData;
  final TextEditingController textController;
  final Function() onSend;
  final Function()? onConnect;

  const HomeScreen({
    Key? key,
    this.ports = const [],
    this.status = "Idle",
    this.errorMessage  = "",
    this.port,
    this.weightValue = "0.00",
    this.serialData = const [],
    required this.textController,
    required this.onSend,
    this.onConnect,
  }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.white,
      appBar: AppBar(
        backgroundColor: Colors.white,
        elevation: 0,
        title: Row(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: [
            Container(
              child: Text(
                'DATE TIME',
                style: TextStyle(
                  color: Colors.orange,
                  fontSize: 16,
                  fontWeight: FontWeight.bold,
                ),
              ),
            ),
            Text(
              errorMessage.isEmpty ? 'OK' : errorMessage,
              style: TextStyle(
                color: Colors.red,
                fontSize: 16,
                fontWeight: FontWeight.bold,
              ),
            ),
          ],
        ),
      ),
      body: SingleChildScrollView(
        padding: EdgeInsets.all(20),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.center,
          children: [
            // CÂN NẶNG Section
            Column(
              children: [
                Text(
                  'CÂN NẶNG',
                  style: TextStyle(
                    color: Colors.blue,
                    fontSize: 20,
                    fontWeight: FontWeight.bold,
                  ),
                ),
                SizedBox(height: 16),
                Container(
                  width: double.infinity,
                  padding: EdgeInsets.all(20),
                  decoration: BoxDecoration(
                    border: Border.all(color: Colors.black, width: 2),
                    borderRadius: BorderRadius.circular(12),
                  ),
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Text(
                        weightValue,
                        style: TextStyle(
                          color: Colors.red,
                          fontSize: 32,
                          fontWeight: FontWeight.bold,
                        ),
                      ),
                      SizedBox(height: 30),
                      Text(
                        'KG',
                        style: TextStyle(
                          color: Colors.blue,
                          fontSize: 32,
                          fontWeight: FontWeight.bold,
                        ),
                      ),
                    ],
                  ),
                ),
              ],
            ),

            SizedBox(height: 40),

            // KẾT NỐI Button
            Container(
              width: double.infinity,
              decoration: BoxDecoration(
                color: Colors.yellow[300],
                borderRadius: BorderRadius.circular(8),
              ),
              child: Material(
                color: Colors.transparent,
                child: InkWell(
                  onTap: onConnect,
                  child: Padding(
                    padding: EdgeInsets.symmetric(vertical: 20),
                    child: Text(
                      status == "Connected"? 'DISCONNECT' : 'CONNECT',
                      textAlign: TextAlign.center,
                      style: TextStyle(
                        color: Colors.green,
                        fontSize: 18,
                        fontWeight: FontWeight.bold,
                      ),
                    ),
                  ),
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}