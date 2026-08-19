# Party Roster

Command line party roster written in C. Focus on nested structs, pointers, memory allocation, input handling, multi-file compilation, linking, and enums.

## How to Use

The program launches into a command-line menu with selectable options. You can select an option by entering the corresponding number and then pressing enter. Entering "q" case-insensitive will generally be go back, or quit to exit the program properly. The numbered options will manipulate data found within a party array in some fashion with inventory being the most dynamic.

1. **Add Character**  
Prompts the user for name and job. Prints a summary of the character at the end.
2. **View Party**  
Prints a list of all the party members. Input the number of a party member to view a corresponding character summary.
3. **Change Job**  
Prompts the user to select a character allowing them to select a different job. Prints a summary of the character at the end.
4. **Remove Character**  
Prompts the user to select a character to remove from party. Prints the bytes of memory freed that were dynamically allocated.
5. **Sort Characters**  
Prompts the user to select what to sort by then the direction. Prints the new order of the party at the end.
6. **Open Inventory**  
Prompts the user to select a character. Input the number of a character to print a list of the contents of their inventory.
7. **Add Item**  
Prompts the user to select a character. Upon selecting a character a list of items will be printed. Input the corresponding number of an item to add it to the selected character's inventory.
8. **Remove Item**
Prompts the user to select a character. Selecting a character will print a list of the character's inventory. Input the number to remove the corresponding item.
9. **Quit**  
Frees up all dynamically allocated memory then exits the program.

## Building

Requires GCC and `make`. Tested with MYSYS2/MinGW-w64 UCRT64 on Windows.

```powershell
make
.\main.exe
```

```bash
make
./main.exe
```

Run `make clean` to remove build files.

## Notes

This is the last project I make in this menu loop style. It was kind of getting old after the contact book. Originally the scope of this project was just going to be exploring nested structs, but I ended up branching out into a lot of different concepts. This is by far the most robust thing I have built thus far. I am not happy with some of the design choices and inconsistency regarding the menu. I am however happy with a lot of the structural choices I made for the project which made it easier to understand in the end. 

### Pointers

Pointers seem to be a recurring topic. I don't want to go to into memory allocation yet, but oh boy the limitations in structs. Originally the character struct had inventory as an array. This has big problems though as it turns out because the character struct is actually used in an array of character structs called party. C does not like this as my compiler told me. Now if character wasn't in an array I believe putting inventory as an array at the end of character would have been fine.

So anyways back to the pointer bit because it's important here. Instead of shoving the dangerous array into the character struct we just made a pointer. Now this is honestly what broke my brain for a bit due to how arrays normally work. For some reason in my head I just assumed passing a pointer meant you were directly changing the values at a memory address and it was enough. Well turns out functions like `realloc()` spit out a pointer. Well if we want to change pointer we passed as an argument we need to go another layer deeper. Behold `**ptr` in all it's glory. A memory address that hold a memory address. Now when we pass this we can dereference it which will allow us to access the memory address of our dynamic array not the local version in the function.

This introduces a lot of syntax issues that basically can be solved by wrapping the dereference in parenthesis like `(*ptr)[index]`. Essentially this is what locks in the idea that an array is just a memory address and in this case we need to keep track of it's size manually. I'm sure I'll be talking about memory again.

### Multiple Files

This is the first project I decided having multiple files would help me keep my sanity. Well it kind of ended up doing the opposite for a bit. Honestly I don't even know where to begin. This entire process began with me manually copying segments of my code over to individual files which I won't name because they changed several times over the course of the project. I will mention though that this was a learning process where I had to think more deeply about specific roles.

I guess we can start to talk about the problems. File dependency is not something to mess with in C. I'm going to be completely honest here, I had issues I didn't understand and fixed them by simply move things around. I believe what was happening is basically a header file would need a specific type for let's say character, and originally I had the character struct in the character header. Well turns out this header or file also needs that one creating a loop where something isn't initialized properly creating confusing errors. The simple solution to this is basically to just have a types header which can be used instead of multiple headers. Another solution seemed to be thoroughly analyzing what each file needed carefully and minimizing the includes and using lighter headers like `<stddef>` if you only need things like `size_t` and `NULL`.

Outside of the dependencies it's probably worth mentioning the `#ifndef` macro. This will basically only do what is in it's body which is ended with `#endif` if what you input after isn't defined. This seems to only check when the preprocessor reads the line, so if you put `#define` on the line after you can make it, so the file won't be read again, but will be read this time. This is important because if you include the same header multiple times in your project everything in it like types will get redefined which is bad.

Okay time to talk about an external tool that made all of this possible. Make basically let's you automate the building process. You create a file named a Makefile which the make command will read and run. I'm not going to explain specifics about the syntax, but essentially you give a build target then list requirements for that target. On the line below you enter the console commands to obtain that build target. If requirements that are listed are missing and there's targets for them listed it'll follow those requirements and commands to build them. Make will only build targets that have requirements that are newer than the build target. It's also worth noting GCC has compiler flags to generate .d files which can be `-include` anywhere in your Makefile to have headers added on as dependencies.

### Input Validation

I became very familiar with the way `fgets()` functions in this project as I stopped using `scanf()` entirely. `fgets()` has a lot of issues revolving around the way it deals with the stdin buffer. I created a pretty robust function for generating error codes and properly filtering these inputs including a variation for integers.

The core thing to remember with `fgets()` is that if you enter one character you need a buffer size of 3 bytes to store everything `fgets()` outputs. The reason for this is that in addition to your character `fgets()` will insert a new line character `\n` and terminator `\0` . The terminator has to exist in a string to signify the end of it. However in the case you have a buffer size of 2 bytes, the new line character does not need to exist, but this is where a "bug" if you want to call it that exists. The new line character is not simply deleted. Instead it is pushed back into the stdin buffer for `fgets()` to pull again. What will happen is the next time `fgets()` is called it will immediately exit eating the input, because `\n` is the same as hitting enter. This is intended behavior and is why cleaning the stdin buffer is important. In my code I did this by checking if the string contained `\n` to determine whether or not this needed to happen.

I purposely invalidated the input whenever it was too long by overwriting it with a bunch of `\0` because it meant what the user wanted would not be stored. In the case that the `\n` character was found I would just simply replace it with a `\0` to remove the forced annoyance when formatting text. I also adopted the concept of early returns here using guard clauses as it immensely helped the readability of the code by preventing deep nesting.

Now most of the project revolves around numerical input, and this is simply something I would probably never do again because it feels horrible from a user perspective. Instead I would have the user type commands instead with maybe some advanced form of fuzzy matching I'd have to learn how to make. With that being said I did make a variation of the above function that utilizes `atoi()`. In the future I would probably explore other methods like `strtol()` because I had to utilize guard clauses to account for a lot of scenarios `atoi()` couldn't handle. 

To actually handle the menu inputs and validate whether or not the input was valid for that menu I had to create a validation function that was flexible enough to be reused. This function would basically filter the status of the `clean_input_int` and normalize user input for menu options. This would return a status that was more simple that would tell the menu whether or not to proceed that would be used in switch statements.

### Enums

Enums are probably one of the best things I have discovered. They are a more memory safe version of define, but have a lot of special properties that in my opinion make them a better default choice. Like define enums are also substituted at compile time and are safe to pass as array sizes. Enums are basically text that represents either the assigned integer or an automatically assigned one. Enums will take their list and assign values starting from 0 incrementing by 1 to each entry.

Due to this nature the last entry in an enum will represent the total number of entries in the enum. Because the values start at 0 without manual assignment the values below 0 can be used to represent invalid or error without changing the total number of entries represented by the previously mentioned final entry. These two concepts are what people call "sentinels" I believe and are very useful to know. That being said manual assignment like previously mentioned will make these methods not work as intended without manual intervention.

With this being in mind you the programmer sees text, but the actual machine sees numbers. This makes enums very efficient for things like switch statements. It is worth noting however enums can only represent integers, if you need a float like PI at compile time your only option is `#define`. In general though I would avoid using define and instead use constant variables if you need different types of constants. The reason for is that define has no type safety, but actual variables do, and they also respect scope. It is very important though to note that enums and define are not variables, they are replacements the compiler does.

### Dynamic Memory

I talked about this a bit in the pointer section, but you probably shouldn't dynamically allocate memory directly to an array in a struct that is in another array. Instead you should store the memory address of the dynamically allocated memory in those structs. Now I am not certain, but I think because of caching it could be better to have dynamic memory in the struct directly as long as the struct is not inside an array and the array you are allocating to is at the end of the struct.

On the topic of actually allocating memory I found the hardest part of this entire project was using `realloc()` to reallocate memory when removing an item from the inventory. The entire issue revolves around having to shift an array to actually "delete" a value. If you do this before running `realloc()` and it returns `NULL` meaning it failed your inventory is now destroyed and the allocation wasn't properly reduced. If you "delete" the value after `realloc()` technically the memory has been freed so you are accessing memory that is undefined. You also have the constraints of needing the `inventory_size` reduced directly after shifting the array, and obviously being increased in the case of NULL.

The solution to this problem is to store the value or structure in this case that you are about to delete into a buffer. Then shift the array, reduce the size, write `realloc()` to a temp pointer. In the case that `realloc()` is null, we have to shift the data to the left to reverse what we did stopping before we get to where the value we destroyed is supposed to be. We then replace the index with the buffer we created effectively recreating the original array. In the care of success we simply just write the output which we stored in a temp pointer to our inventory pointer.

Dynamically allocated memory comes with the need to free it when you are no longer using it. In my project I covered removing character, removing items, and quitting the program. Whenever you free memory it is a good habit to set the memory to `NULL`. I actually used this method to initialize pointers ironically in my `add_character` function. `inventory_size` when creating a character is 0. `malloc()`  can do different things depending on compiler, so I just ended up manually setting the pointer to `NULL` which can be processed by  `realloc()`.

### Refactoring

This is by far the largest code base I have worked with, and it really did warrant multiple files. I also felt immediate punishment for a lot of design decisions I made early on in regards to functions. I also still have ideas that were not implemented, but I will look to design around in the future. Because of these facts there was a point where I had to remake a significant amount of code generating problems I had to work through.

Early on I would call my `clear_terminal` and `wait_enter` methods into functions that existed just to display text. This has the major disadvantage of calling these functions when I only wanted to print text. I have adopted the belief that individual tasks should have specialized functions if they are commonly used. In the case there is a routine of tasks I need to build a separate function. Now I did not do this in my project entirely, but I did separate the tasks to an extent. I could have gone further and separate display entirely from data management which I believe is what professional code bases usually do.
