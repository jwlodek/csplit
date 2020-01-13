
#!/bin/bash

cd ../../..
if [ ! -d "csplit-docs" ]
then
git clone https://github.com/jwlodek/csplit-docs
fi
cd csplit
python3 -m mkdocs build -d ../csplit-docs
cd ../csplit-docs
git add -A
DATE=$(date)
git commit -m "Update csplit docs $DATE"
git push