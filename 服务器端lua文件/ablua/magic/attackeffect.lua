function 攻击特效(charaindex, data)
	local 特效动画ID = other.atoi(other.getString(data, " ", 2));
	char.setWorkInt(char.getCharPet(charaindex,0),"攻击特效",特效动画ID);
	--char.setWorkInt(charaindex,"攻击特效",特效动画ID);
end

function main()
	magic.addLUAListFunction("攻击特效", "攻击特效", "", 1, "[gm 攻击特效 动画编号]")
end
