## Chrome Injector

The modified code from [Chrome-App-Bound-Encryption-Decryption](https://github.com/xaitax/Chrome-App-Bound-Encryption-Decryption/), converted into a DLL format, with injector written in C++, allows us to retrieve the App Bound key from any directory and without administrator privileges

Undetectable by windows defender in 14.04.2025

Build app_bound.cpp in visual studio as .dll x64 and ChromeInjector.cpp as .exe x64

Also it can be used for msedge and brave, but in original code we have some error with msedge browser [issue](https://github.com/xaitax/Chrome-App-Bound-Encryption-Decryption/issues/10) if u can fix it i will be happy to see your solution 
![изображение](https://github.com/user-attachments/assets/9b16c534-37f7-4d43-b04c-68cf308be1ca)

For use brave or edge change line number 361 in app_bound.cpp and process name variable in ChromeInjector.cpp



There is no publicly available source code for a program that can fully retrieve the App Bound key without administrator privileges. The method used by @xaitax also requires administrator rights to move into the Program Files directory. 

Ref: [Chrome-App-Bound-Encryption-Decryption](https://github.com/xaitax/Chrome-App-Bound-Encryption-Decryption/)

## Update: 

So @xaitax posted my method on his repo with a better implementation. I’ll be releasing a cookie decryptor based on this method soon. Don’t forget to star my repository and subscribe — only unique content will be posted here

## For educational purposes only.


