# Autobots

The BSD terminal game
[robots](https://en.wikipedia.org/wiki/Chase_(video_game))
is a turn based single player game.
`autobots` is an expert system that plays `robots`.

## Coding

`autobots.c`
- creates a pseudo terminal
- fork/exec `robots`
- parses the tty contol sequences
- calls `strategy()` to determine the next move

`strategy.c`
- codes one good strategy
- performs better than the running robots automaticaly with `robots -A`

Build `autobots` calling `make`

## Computational results

My high store using `autobots` is 8860.