from opcua import Client 
from node import *

client = Client("opc.tcp://ole-VirtualBox:4840/")
client.connect()

sn = ServerNodes()
sn.build_list(Node(client.get_objects_node(), None))
sn.show_hierarchie()
sn.node_list[83].get_node_value()


