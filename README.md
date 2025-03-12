# Janome MC8000 ROM reader

This is a basic sketch to dump the data from a ROM for the Janome MC8000 sewing machine.  

See <https://www.mawds.co.uk/janome/janome.html> for further details

Use putty, with logging enabled, to connect to the Arduino.  Then press R to read the ROM

Use the following to convert the captured data to a ROM file  

```
python convert_to_bin.py --input <logfile> --output my_rom.bin
```

(There are a lot of `delay()`s in the code - these are quite conservative, and can probably be reduced)


