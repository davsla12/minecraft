import json
with open("/home/david/Documents/minecraft-data/data/pc/1.8/protocol.json","r") as f:
  data = json.load(f)

with open("./mapping.cpp","w") as m:
  m.write("#include \"mapping.h\"\n")
  m.write("std::map<std::string,std::map<std::string,int>> toClient = {\n")
  for state in data:
    if(state != "types"):
      m.write("  {\n")
      m.write("    \""+state+"\", {\n")
      print(state)
      for key in data[state]:
        print(key)
        for id,name in data[state][key]["types"]["packet"][1][0]["type"][1]["mappings"].items():
          if(key == "toClient"):
            print("-->"+name+":"+id)
            m.write("      {\""+name+"\","+id+"},\n")
          else:print("-->"+id+":"+name)
    if(state != "types"):m.write("  }");m.write("  },\n")
  m.write("};\n")
#for id,name in data["status"]["toClient"]["types"]["packet"][1][0]["type"][1]["mappings"].items():
#  print(id+":"+name)
