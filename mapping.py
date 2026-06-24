import json
with open("/home/david/Documents/minecraft-data/data/pc/1.8/protocol.json","r") as f:
  data = json.load(f)

with open("./mapping.h","w") as m:
  m.write("#ifndef MAPPING_H\n")
  m.write("#define MAPPING_H\n")
  for state in data:
    if(state != "types"):
      print(state)
      m.write("//" + state + "\n")
      for key in data[state]:
        print(key)
        for id,name in data[state][key]["types"]["packet"][1][0]["type"][1]["mappings"].items():
          m.write("#define " + (state + "_" + key + "_" + name + " " + id + "\n").upper())
          #if(key == "toClient"):
            #print("-->"+name+":"+id)
            #m.write("      {\""+name+"\","+id+"},\n")
          #else:print("-->"+id+":"+name)
    #if(state != "types"):m.write("  }");m.write("  },\n")
  m.write("#endif")
#for id,name in data["status"]["toClient"]["types"]["packet"][1][0]["type"][1]["mappings"].items():
#  print(id+":"+name)
