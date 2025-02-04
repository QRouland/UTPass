# 0.0.3: Git Initial Support and Move to RNP
- Port app to Focal
- Improve UI :
  * Follow human interface guidelines
  * Fix various components color to work with black theme 
- Rewrite of Pass Plugin:
  * Move from GPGMe to RNP for GPG operations due to issues running GPG agent in a confined app
  * Improve multithreading code for GPG operations
- Add Git HTTP and HTTP AUTH clone for password store import feature
- Add delete password store feature

# 0.0.2 :  Added translations 
- Added French by Anne17  and Reda 
- Added Catalan by Joan CiberSheep 
- Added Spanish by Advocatux and Reda 

Thanks to all the translators !

# 0.0.1 : Initial Release  
- Import of gpg keys via file
- Suppression of gpg keys
- Import of password via a password store zip
- Password decryption
- Password copy to clipboard
