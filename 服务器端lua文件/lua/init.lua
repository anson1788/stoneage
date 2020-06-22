function 脚本加载(路径)
	return dofile(路径);
end

function 主函数()
	脚本加载("./data/lua/onlineshop/main.lua");
	--时间种子
	math.randomseed(os.time());
end

主函数()

