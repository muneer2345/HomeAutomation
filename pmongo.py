from pymongo import MongoClient

#connection = MongoClient("mongodb://127.0.0.1:27017/iot")
connection = MongoClient('localhost', 27017)
db = connection.iot.dosiot

#ledstatus = db.find({'device':'LED'})




#for i in ledstatus:
#		print (i["value"])


#print ledstatus['value']

newquote ={'device':'temperature','value':'999'}

db.insert(newquote)

newquote ={'device':'USRLED','value':'USRLED0'}

db.insert(newquote)

newquote ={'device':'LED','value':'LED0'}

db.insert(newquote)
#query ={'device': 'temperature'}

#db.update(query, {'$set': {'value': '77'}})
#newquote ={'device':'LED','value':'OFF'}
#db.insert(newquote)
# newquote ={'device':'led','value':'on'}

#db.insert(newquote)


