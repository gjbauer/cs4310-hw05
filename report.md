How does your program deal with dynamic memory allocation?
    Well, MY program unfortunately calls calloc and malloc, because I didn't quite make it far enough to be able to replace all of those calls with mmap...

Given that the kernel will only really allocate 4kB pages that a program writes to, how much memory gets allocated for your program's anonymous mappings when running on xmas.txt? Why?
    Well, given that I am using malloc on everything but opening up the file, and not using the syscalls directly, I am going to guess that it is not too far off from the memory required which I am requesting. Otherwise, I am sure that a lot of pages will be used.

How much memory gets allocated for anonymous mappings when your program runs on meditations.txt?
    Same answer that I gave one the previous answer.

What is the asymptotic  complexity (big O) of your word counting in terms of the number of distinct words in the input file? Why?
    It must be incredibly high. Let's think about it. I pull each and every line, line-by-line. Then I split up each line word-by-word. Then, I have to iterate over ever single previous stored word to see if it matches. I don't know n^3? Awful. Its terrible.
