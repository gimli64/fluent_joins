This is a demo version of PDGF. Copyright (c) 2013, www.paralleldatageneration.org. All rights reserved. 

QUICKSTART:
-----------
Recommended java version: 1.7 64Bit.
(minimum 1.6 32Bit, but 32Bit VM's have limitations regarding the maximum useable memory)

Default start:
cd /path/to/PDGFEnvironment
java -XX:NewRatio=1  -jar pdgf.jar -l ssb-schema.xml -l ssb-generation.xml -s

same as above, but using long version of commands:
cd /path/to/PDGFEnvironment
java -XX:NewRatio=1 -jar pdgf.jar -load ssb-schema.xml -load ssb-generation.xmlloseWhenDone -start 

Start generation with a specific scale factor:
java -XX:NewRatio=1  -jar pdgf.jar -l ssb-schema.xml -l ssb-generation.xml -s -sf 2000

To generate only one or more specific tables (Table names are case sensitive!) specify them as start command parameters:
java -XX:NewRatio=1  -jar pdgf.jar -l ssb-schema.xml -l ssb-generation.xml -s LINEORDER PART

Recommended JVM memory settings Xms and Xmx for 85 workers:
java -Xms1300m -Xmx1300m -XX:NewRatio=1 -jar pdgf.jar -l ssb-schema.xml -l ssb-generation.xml -w 85 -s

Example: put all together (generate with 85 workers and scale factor 2000 only the tables CustomerMgmt and FINWIRE):
java -Xms1300m -Xmx1300m -XX:NewRatio=1 -jar pdgf.jar -l ssb-schema.xml -l ssb-generation.xml -w 85 -s LINEORDER PART -sf 2000

Description of config files:
----------------------------
ssb-generation.xml              General generation config for all data sets.
ssb-generation-queries.xml      Generation config for Q1.1. queries.
ssb-schema.xml                  SSB compatible schema definiton config.
ssb-schema-skewed.xml           SSB schema definition config with skew as defined in the ICPE'13 paper
ssb-schema-skewed-ref-part.xml  SSB schema definition config with skew on references to PART in LINEORDER

 
Exclusion of tables from the generation
----------------------------------------
If not all tables defined in the generation xml should be generated, the names of the tables which should be generated can be appended
to the 'start' option:
java -XX:NewRatio=1 -jar pdgf.jar -load ssb-schema.xml -l ssb-generation.xmlloseWhenDone -start LINEORDER PART
In this case, only LINEORDER and PART will be generated.


File encoding (charset) and system specific line.Seperators
------------------------------------------------------------
The default encoding used by PDGF is 'UTF-8' for all output files, if not overridden within the xxGeneration.xml files (Example: <output name="SomeOutputPlugin"> <charset>UTF-8</charset> </output>).
The default line separator is operating system dependent. Unix: LF, Windows: CRLF, MAC: CR

You can override this framework default behavior using two ways.
1. Start the JVM with an property -D<PropertyName>="<Value>":
	java -DDEFAULT_LINESEPARATOR="CRLF"c -jar pdgf.jar
	java -DDEFAULT_CHARSET="UTF-8" -jar pdgf.jar
	or both
	java -DDEFAULT_CHARSET="UTF-8" -DDEFAULT_LINESEPARATOR="CRLF" -jar pdgf.jar
2. create a file named "Constants.properties" in ..\PDGFEnvironment\ if it not already exists and add the two lines:
DEFAULT_LINESEPARATOR=CRLF
DEFAULT_CHARSET=UTF-8

 
DETAILED EXPLANATION:
----------------------
To run PDGF, open a terminal, change into the PDGFEnvironment directory and run:

cd /path/to/PDGFEnvironment
java -XX:NewRatio=1 -jar pdgf.jar

This will open pdgf in interactive mode and optimizes the Heap space for young generation.
To exit the pdgf shell type 'exit'.
You can get a list of available commands with 'help' or  'h'. 
To get help for an specific command type: help <cmdName> where cmdName is the command name.
Example: 'help l'  will show help for the load command.

To run the ssb data generation, you have to enter at least these commands (in this order, l for load, s for start and w to set worker count):

l ssb-schema.xml
l ssb-generation.xml
s

The important thing is to first 'load' the two XML files in the given order and to 'start' when all other commands were entered.

After loading the two files and before starting generation, you can execute some other commands, for example:
sf <x> to change the scale factor (causes reloading of the XML files)
s <Table> <Table> ... <Table> (to generate only the specified tables)

For subsequent testing, it is better to use startup options, so the same commands do not need to be entered several times.
This can be done by appending the jar execution command line with commands listed by 'help' (of the form -{command-name})

To run the minimum example from above by only using the command line, this has to be done:
cd /path/to/PDGFEnvironment
java -XX:NewRatio=1 -Xmx512m -jar pdgf.jar -load ssb-schema.xml -load ssb-generation.xml -start
This command will generate the data and will leave you in the pdgf shell. 

To come back to the command prompt of your shell you can use 'closeWhenDone': 
java -XX:NewRatio=1 -Xmx512m -jar pdgf.jar -load ssb-schema.xml -load ssb-generation.xmlloseWhenDone -start
This time, pdgf will automatically exit after data generation finished and will return to the shell command prompt.


