import json
with open("/home/david/Documents/minecraft-data/data/pc/1.8/protocol.json","r") as f:
  data = json.load(f)

with open("./mapping.cpp","w") as m:
  m.write("std::map<std::string,std::map<std::string,int>> toClient = {\n")
  for state in data:
    if(state != "types"):
      m.write("  {\n")
      m.write("\""+state+"\"")
      print(state)
      for key in data[state]:
        print(key)
        for id,name in data[state][key]["types"]["packet"][1][0]["type"][1]["mappings"].items():
          if(key == "toClient"):print("-->"+name+":"+id)
          else:print("-->"+id+":"+name)
  m.write("};\n")
#for id,name in data["status"]["toClient"]["types"]["packet"][1][0]["type"][1]["mappings"].items():
#  print(id+":"+name)
