g_Game_GUI_Item = 
{
	{ 'DXFile\\tank2.plg', 0.75,0.75,0.75,  -- �s�łS��
	math.random(),math.random(),math.random(), -- �S�C���ɵ���������
	0,0,0 -- ���D�S��
	},
	
};

for k,v in pairs(g_Game_GUI_Item) do
	Load3DModule( k, v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10]  ) --�������Π����w�����ID,ģ���ļ�,�s�Ŵ�С,���mʹ��WorldPos�M�и�����������
end

RegisterEvent("GameEventHandler")
function GameEventHandler( id, event )
	if id ~= 0 then
		local fun = g_Game_GUI_Item[id]['Hanlder'];
		if nil ~= fun then fun(event) end;
	end
end
