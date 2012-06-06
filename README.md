Prototype of an ionisation chamber-based radiation detector for [OpenRelief](http://openrelief.org). Designed around a Nanode but could easily make use of another Arduino-compatible board with Ethernet. 

Links:

 * [SolderPad](http://solderpad.com/9600/ORion)
 * [GitHub](https://github.com/9600/ORion)
 * [Write up](http://www.designspark.com/content/treacle-tin-radiation-detector).

Note that this design/code is alpha quality. Known issues include:

 * Code ceases to print measurements out to the serial port if Ethernet is not connected
 * No scaling in code
 * Chamber design needs optimising, e.g. tweaking its capacitance
 * Prototype makes use of 3 x 9v (PP3) batteries for chamber bias, and this needs replacing with an Arduino shield boost converter
