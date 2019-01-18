today = `date +%Y%m%d`

tar -cvf /home/backup/BAK_david.tar.`date +%Y%m%d` --exclude=dat --exclude=log /home/david
