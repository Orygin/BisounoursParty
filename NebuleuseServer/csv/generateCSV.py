#Generates a CSV file from the json files present in this very folder
import os
import json
import re

files = [f for f in os.listdir('.') if re.match(r'[0-9]+.*\.json', f)]
stats = list()

for f in files:
	of = open(f)
	dof = of.read()
	of.close()
	objo = json.loads(dof)
	stats.append(objo)
	
out = open("output.csv", "w")
out.write("Date,\"Time Played\",\"Kills Total\",\"Player Count\",\"Maps Total\",\"Bong Smoked\"\n")

for s in stats:
	stat = s['Stats']
	Date = stat['date']
	formatedDate = Date[2] + Date[3] + "/" + Date[4] + Date[5] + "/" + "20" + Date[0] + Date[1]
	out.write(formatedDate)
	out.write(",")
	out.write(stat['timePlayed'])
	out.write(",")
	out.write(stat['killsTotal'])
	out.write(",")
	out.write(stat['playerCount'])
	out.write(",")
	out.write(stat['mapsTotal'])
	out.write(",")
	out.write(stat['bongSmoked'])
	out.write("\n")
	
out.close()