Given a word and a dictionary, finds those words in the dictionary that are anagrams of the given word.
Made specificially for a code challenge and for the particular dictionary used in that code challenge, NOT suitable for general use.

Usage:
```
./a.out /path/lemmad.txt word
```
For phrases that contain spaces, the use of quotes is optional:
```
./a.out /path/lemmad.txt "phrase with spaces"
./a.out /path/lemmad.txt phrase with spaces
```
(but without quotes, consecutive spaces such as "abc &nbsp; def" will most likely be reduced into one space by the shell).
