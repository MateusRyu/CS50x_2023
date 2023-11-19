from cs50 import SQL

db = SQL("sqlite:///songs.db")

fields = [
    "danceability",
    "energy",
    "key",
    "loudness",
    "speechiness",
    "valence",
    "tempo",
    "duration_ms"
    ]

print(80 * '#')
for field in fields:
    print(field)
    rows = db.execute(f"SELECT MIN({field}) AS 'MIN', AVG({field}) AS 'AVG', MAX({field}) as 'MAX' FROM songs;")
    print(rows[0])
    avg = rows[0]["AVG"]
    rows = db.execute(f"SELECT COUNT(name) AS 'up' FROM songs WHERE {field} > {avg};")
    songs = {"up": rows[0]["up"]}
    rows = db.execute(f"SELECT COUNT(name) AS 'low' FROM songs WHERE {field} < {avg};")
    songs["low"] = rows[0]["low"]
    print(songs)

artists = {}
rows = db.execute("SELECT artists.name as 'artist', count(songs.artist_id) as 'count' FROM songs join artists on songs.artist_id = artists.id group by artists.name;")
for row in rows:
    if row["count"] > 2:
        artists[row["artist"]] = row["count"]

rows = db.execute("SELECT name FROM songs where name LIKE '%feat.%';")
for row in rows:
    name = row["name"].split("feat.")
    name = name[1].strip(")")
    name = name.strip("")
    name = name.split(" & ")
    if len(name) == 1:
        feat = name
    else:
        feat = name[0].split(", ")
        feat.append(name[1])

    for artist in feat:
        if artist in artists:
            artists[artist] +=1

names  = list(artists.keys())

for name in names:
    print(f"{name}: {artists[name]}")

