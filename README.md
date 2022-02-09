# RP2040 ARM repeater controller
**This is a work in progress for a fully functional ARM 32bit based repeater controller written in `c`**

## Who's this for?
**Anyone who want's to build a repeater controller based on the RP2040 ARM CPU**

## Is there a hardware design?
**Yes, but at the moment I don't have a PCB finished. All testing is on stripboard**

## Is this software in use on any repeaters?
**Yes, GB3VW uses this software**

### Why would you do this when there's other options for repeater controllers?
**It's simple, sometimes you need something unique and need to roll your own**

### What features does this controller have so far?
- [ ] Advanced Allstar-Asterisk intergration
- [x] CTCSS decode input (External tone decoder required)
- [x] DTMF control
- [x] Internet gateway
- [x] Morse code beacon ID
- [x] RSSI courtesy tone reports
- [ ] Serial interface
- [ ] Speech
- [x] Timeout
- [x] Toneburst decode input (NE567 required)