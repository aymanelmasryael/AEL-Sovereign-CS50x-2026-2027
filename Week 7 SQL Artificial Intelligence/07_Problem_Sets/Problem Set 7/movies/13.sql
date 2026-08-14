SELECT DISTINCT p.name
FROM people AS p
JOIN stars AS s ON s.person_id = p.id
JOIN movies AS m ON m.id = s.movie_id
JOIN stars AS s2 ON s2.movie_id = m.id
JOIN people AS p2 ON p2.id = s2.person_id AND p2.name = "Kevin Bacon" AND p2.birth = 1958
WHERE p.name != "Kevin Bacon"