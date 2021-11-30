
  var oWSS = new ActiveXObject("WScript.Shell");

  var regpath_User = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\RegisteredOwner";
  var regpath_Computer = "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName\\ComputerName";
  var regpath_Processor = "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\PROCESSOR_IDENTIFIER";
  var regpath_OS = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\ProductName";

  //oWSS.RegRead(regpath_OS);
var a, b, fs;
var filename;
filename = "hello.txt";
ForAppending = 8;

fs = new ActiveXObject("Scripting.FileSystemObject");
b = fs.CreateTextFile(filename, true);
b.Close();
alert("Created");

a = fs.OpenTextFile(filename, ForAppending);
a.WriteLine(oWSS.RegRead(regpath_User));
a.WriteLine(oWSS.RegRead(regpath_Computer));
a.WriteLine(oWSS.RegRead(regpath_Processor));
a.WriteLine(oWSS.RegRead(regpath_OS));
//a.Close();
alert("Written");

batName = "CopyFile.bat"
x = fs.CreateTextFile(batName, true);
x.Close();
alert("Created bat");

y = fs.OpenTextFile(batName, ForAppending);

var fso=new ActiveXObject("Scripting.FileSystemObject");
var path=fso.GetAbsolutePathName(".");
var shareNetwork = " \\\\MISA0136\\Users\\Public\\MAC_address"
//fso=null;
alert(path);

// a.WriteLine(oWSS.RegRead(regpath_Processor));
// a.WriteLine(oWSS.RegRead(regpath_OS));
// var path = "C:\\Users\\dungphan16499\\Desktop\\txt.bat"
// oWSS.run(path)

//function networkInfo() {

  var wmi = new ActiveXObject("WbemScripting.SWbemLocator");
  var service = wmi.ConnectServer(".");

  e = new Enumerator(service.ExecQuery("SELECT * FROM Win32_NetworkAdapterConfiguration WHERE IPEnabled = True"));

  for (; !e.atEnd(); e.moveNext()) {
     var s = e.item();
     var macAddress = unescape(s.MACAddress);
  }

  var text = macAddress;
  var MAC = text.replace(/:/g, "-");
  alert(MAC);
  var newPath = shareNetwork+ "\\"+MAC;
  y.WriteLine("mkdir "+ newPath);
  alert("mkdir "+ newPath);
  y.WriteLine("xcopy " + path + "\\hello.txt" + newPath);
  y.WriteLine("pause");
  alert("Written bat");

  //a.WriteLine(macAddress);
  a.Close();
//  return macAddress;
//}

//alert(networkInfo());
//readFromRegistry("ProductName");
//alert(readFromRegistry("ProductName"))

