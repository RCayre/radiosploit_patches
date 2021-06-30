RadioSploit 1.0 - Patches
=========================

This repository contains a set of [InternalBlue](https://github.com/seemoo-lab/internalblue) patches for the BCM4375B1 Bluetooth controller, allowing to sniff and inject Zigbee, Mosart and Enhanced ShockBurst packets from a Samsung Galaxy S20 smartphone. This patches add two new HCI commands allowing to interact with the previously mentioned protocols by diverting the BCM4375 Bluetooth controller. An Android application named [RadioSploit](https://github.com/RCayre/radiosploit) provides a Graphical User Interface (GUI) to manipulate these new commands.

This project is a Proof of Concept developed in the context of a research work aiming at exploring the feasibility of cross-protocol pivoting attacks. If you need additional details, we have published multiple papers about it:

   * Romain Cayre, Florent Galtier, Guillaume Auriol, Vincent Nicomette, Mohamed Kaâniche, Géraldine Marconato. POSTER: Cross-protocol attacks : weaponizing a smartphone by diverting its Bluetooth controller. *14th ACM Conference on Security and Privacy in Wireless and Mobile Networks (WiSec 2021)*, Jun 2021, Abu Dhabi (virtual), UAE. **\[en\]**

   * Romain Cayre, Florent Galtier. [Attaques inter-protocolaires par détournement du contrôleur Bluetooth d'un téléphone mobile](https://hal.laas.fr/hal-03221148). *GT Sécurité des Systèmes, Logiciels et Réseaux*, May 2021, En ligne, France. **\[fr\]**

   * Romain Cayre, Florent Galtier, Guillaume Auriol, Vincent Nicomette, Mohamed Kaâniche, et al. [WazaBee: attacking Zigbee networks by diverting Bluetooth Low Energy chips](https://hal.laas.fr/hal-03193299). *IEEE/IFIP International Conference on Dependable Systems and Networks (DSN)*, Jun 2021, Taipei (virtual), Taiwan. **\[en\]**

   * Romain Cayre, Florent Galtier, Guillaume Auriol, Vincent Nicomette, Geraldine Marconato. [WazaBee : attaque de réseaux Zigbee par détournement de puces Bluetooth Low Energy](https://hal.laas.fr/hal-02778262). *Symposium sur la Sécurité des Technologies de l'Information et des Communications (SSTIC 2020)*, Jun 2020, Rennes, France. pp.381-418.**\[fr\]**

The patches have been generated using the [Nexmon framework](https://github.com/seemoo-lab/nexmon) (especially the "bluetooth-wip" branch). Please refer to the corresponding documentation if you want to compile it by yourself.

This application is released as an opensource software using the MIT License.

How to patch the Bluetooth controller ?
---------------------------------------
* First, you need to root your Samsung Galaxy S20 smartphone. The patches should also work with Samsung Galaxy S10, but it has not been tested.
* Install **adb** and [InternalBlue framework](https://github.com/seemoo-lab/internalblue) on your computer.
* Apply the **Read_Ram** bypass described [here](https://github.com/seemoo-lab/internalblue/blob/master/doc/android.md#bypass-broadcom-read_ram-fix) to enable vendor-specific commands allowing to read, write and launch RAM.
* Install the patches using the following command:
```
sudo python3 patch.py
```
* Install the [RadioSploit Android application](https://github.com/RCayre/radiosploit) or use [InternalBlue](https://github.com/seemoo-lab/internalblue) Command Line Interface to interact with the patched controller.

HCI Commands
-------------

These patches add two new HCI commands to the controller: **Start\_RX** (opcode = 0x2060) and **Start\_TX** (opcode = 0x2061). The received packets are transmitted to the Host using custom HCI events with opcode **0xFF**. 

* If you want to enable Zigbee RX mode, launch the following command :
```
> sendhcicmd 0x2060 01<channel>
```

For example, if you want to select channel 13:
```
> sendhcicmd 0x2060 010d
```

* If you want to enable Mosart scan mode, launch the following command:
```
> sendhcicmd 0x2060 02<channel><allow dongle packets>
```

For example, if you want to select channel 3 without allowing the reception of dongle packets:
```
> sendhcicmd 0x2060 020300
```
 
* If you want to enable Mosart keylogger mode, launch the following command:
```
> sendhcicmd 0x2060 03<channel><keyboard address>
```

For example, if you want to select channel 3 and receive keystrokes from the keyboard using address F0:BF:1E:86:
```
> sendhcicmd 0x2060 0303F0BF1E86
```

* If you want to enable Mosart RX mode, launch the following command:
```
> sendhcicmd 0x2060 04<channel><device address>
```

For example, if you want to select channel 3 and receive packets from the device using address F0:BF:1E:86:
```
> sendhcicmd 0x2060 0403F0BF1E86
```

* If you want to enable Enhanced ShockBurst Scan mode, launch the following command:
```
> sendhcicmd 0x2060 05<channel>
```

For example, if you want to scan channel 8, launch the following command:
```
> sendhcicmd 0x2060 0508
```

* If you want to enable Enhanced ShockBurst RX mode, launch the following command:
```
> sendhcicmd 0x2060 06<channel><device address>
```

For example, if want to receive packets on channel 8 from device using address 11:22:33:44:55:
```
> sendhcicmd 0x2060 06081122334455
```

* If you want to transmit a Zigbee packet, launch the following command:
```
> sendhcicmd 0x2061 01<channel><packet length><packet>
```

For example, if you want to transmit the packet **010d14a7126188f9323300000000d30068656c6c6f9ee0** on channel 13:
```
> sendhcicmd 0x2061 010d14a7126188f9323300000000d30068656c6c6f9ee0
```

* If you want to transmit a Mosart packet, launch the following command:
```
> sendhcicmd 0x2061 02<channel><packet length><packet>
```

For example, if you want to transmit the packet **f0f0f0bf1e8645fcfbfafc7a23** on channel 4:
```
> sendhcicmd 0x2061 02040df0f0f0bf1e8645fcfbfafc7a23
```

* If you want to transmit an Enhanced ShockBurst packet, launch the following command:
```
> sendhcicmd 0x2061 05<channel><packet length><packet>
```

For example, if you want to transmit the packet **aacae906eca4280061010000000000001e748a00** on channel 71:
```
> sendhcicmd 0x2061 054714aacae906eca4280061010000000000001e748a00
```


