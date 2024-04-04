# X86-64 Output
Sonoma 14.4.1 (23E224)

```
ioreg -p IOUSB -l -b | grep -E "@|idVendor|idProduct|kUSBSerialNumberString|kUSBAddress"
```

## Grep'ed listing

```
    | | +-o USB Single Serial@14221000  <class AppleUSBDevice, id 0x10002e598, registered, matched, active, busy 0 (7 ms), retain 13>
    | |       "idProduct" = 21972
    | |       "kUSBSerialNumberString" = "56E3004772"
    | |       "idVendor" = 6790
```
The x86_64 listing doens'yt appear to show `kUSBAddress` as a field.

## Full listing

```
ioreg -p IOUSB -l -b"
```

```
USB Single Serial@14221000  <class AppleUSBDevice, id 0x10002e598, registered, matched, active, busy 0 (7 ms), retain 13>
    | |     {
    | |       "sessionID" = 186249768423684
    | |       "idProduct" = 21972
    | |       "iManufacturer" = 0
    | |       "bDeviceClass" = 2
    | |       "bMaxPacketSize0" = 8
    | |       "bcdDevice" = 1092
    | |       "iProduct" = 2
    | |       "iSerialNumber" = 3
    | |       "bNumConfigurations" = 1
    | |       "Bus Power Available" = 250
    | |       "USB Address" = 10
    | |       "Built-In" = No
    | |       "locationID" = 337776640
    | |       "bDeviceSubClass" = 0
    | |       "bcdUSB" = 272
    | |       "USB Product Name" = "USB Single Serial"
    | |       "PortNum" = 1
    | |       "non-removable" = "no"
    | |       "kUSBSerialNumberString" = "56E3004772"
    | |       "bDeviceProtocol" = 0
    | |       "AppleUSBAlternateServiceRegistryID" = 4295157142
    | |       "IOCFPlugInTypes" = {"9dc7b780-9ec0-11d4-a54f-000a27052861"="IOUSBHostFamily.kext/Contents/PlugIns/IOUSBLib.bundle"}
    | |       "IOPowerManagement" = {"DevicePowerState"=0,"CurrentPowerState"=3,"CapabilityFlags"=65536,"MaxPowerState"=4,"DriverPowerState"=3}
    | |       "Device Speed" = 1
    | |       "kUSBCurrentConfiguration" = 1
    | |       "idVendor" = 6790
    | |       "IOGeneralInterest" = "IOCommand is not serializable"
    | |       "kUSBProductString" = "USB Single Serial"
    | |       "USB Serial Number" = "56E3004772"
    | |       "IOClassNameOverride" = "IOUSBDevice"
    | |     }
```
