# packet structure

```
PACKETNAME\VERSION
<json>
{
    "field": "value"
}
```

**PACKETNAME** defines the type of request/answer
<br>**VERSION** defines the version of the protocol used
<br>**<<text>json>** type in brackets tells the type of data which packet contains, which is defined by the type of packet

in this example packet contains json file

# clientbound packet list
1. **OK**
2. **MESSAGE**

# OK
**OK** is the default returned packet that means sended packet was processed succesfully

```
OK\0.1
```
# MESSAGE

**MESSAGE** contains the text of the message and the data about it

```
MESSAGE\0.1
<json>
{
    "message": "Hello world!",
    "time": "19.03.2026 18:09:55",
    "owner": "@Server"
}
```

_message_  contains the text of the message
<br>_time_ is the date\time string formatted to D.M.Y H:M:S
<br>_owner_ is the tag of person who owns the message

# serverbound packet list
1. **SEND**
2. **GET**

# SEND
**SEND** is used to send messages

if success server should return **OK** packet

```
SEND\0.1
<json>
{
    "message": "Hello world!",
    "owner": "@user"
}
```
_message_ contains the text of the message
<br>_owner_ is the tag of person who sends the message

# GET
**GET** is used to request messages

if success server should return **MESSAGE** packet

```
GET\0.1
<json>
{
    "id": 12
}
```
_id_ contains unique id of the message