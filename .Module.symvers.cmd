cmd_/home/alpha/CS350-Operating-Systems-Project/Module.symvers := sed 's/ko$$/o/' /home/alpha/CS350-Operating-Systems-Project/modules.order | scripts/mod/modpost -m -a   -o /home/alpha/CS350-Operating-Systems-Project/Module.symvers -e -i Module.symvers   -T -