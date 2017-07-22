#!/bin/sh

yes="1"
no="0"

echo "Type commit message: "
read commit_message

echo "Type where to send commit? eg: master,devel etc..."
read where_to

# Force pull is used when your local changes are not anymore needed and you want
# to refetch the data from cloud.
echo "Force pull? eg: 1(yes) or 0(no)"
read force_pull

if [ "$force_pull" -eq "$yes" ]; then
    git fetch --all
    git reset --hard origin/$where_to
    git pull origin $where_to
    git add --all
    git add .
    git commit -m "$commit_message"
    git push origin $where_to
elif [ "$force_pull" -eq "$no" ]; then
    git add --all
    git add .
    git commit -m "$commit_message"
    git push origin $where_to
fi

