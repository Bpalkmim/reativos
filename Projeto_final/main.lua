-- Bernardo Alkmim 1210514

--[[
	Função auxiliar que faz uma oscilação de onda com uma determinada frequência.
	Essa função é útil para guardar as diferentes notas a serem mostradas ao
	usuário para que ele saiba como realizar o exercício.
]]--
function Oscillator(freq)
    local phase = 0
    return function()
        phase = phase + 2*math.pi/rate          
        if phase == 2*math.pi then
            phase = phase - 2*math.pi
        end
        return math.sin(freq*phase)
    end
end

function love.load()
	--Variáveis para controlar o estado da aplicação
	state = 0
	inExample = false
	inTraining = false
	currentIteration = 1
	
	lastTime = love.timer.getTime()
	currTime = love.timer.getTime()
	
	-- Variáveis para a geração dos sons de treinamento
	len = 2
    rate = 44100
    bits = 16
    channel = 1
	amplitude = 0.2

    soundData = love.sound.newSoundData(len*rate, rate, bits, channel)

	-- Notas a serem usadas como base nos exercícios
	-- Agudas: de G3 a F4
	highNotes = {196, 208, 220, 233, 247, 262, 277, 294, 311, 330, 350}
	-- Graves: de D3 a F2
	lowNotes = {147, 139, 131, 123, 116, 110, 104, 98, 93, 87}
	
end

function love.update(dt)
	
	currTime = love.timer.getTime()

	-- Menu principal, com enter apertado: deve ir para o teste de agudos
	if state == 0 and love.keyboard.isDown("return") then
		state = 1
		inExample = true
		
	-- Teste de agudos
	elseif state == 1 then
		-- Ouvir o exemplo
		if inExample and love.keyboard.isDown("o") then
			currentNote = highNotes[currentIteration]
			note = Oscillator(currentNote)
			for i = 1, len*rate do
				sample = note() * amplitude
				soundData:setSample(i - 1, sample)
			end
	
			source = love.audio.newSource(soundData)
			love.audio.play(source)
		-- Esperar pelo usuário
		elseif love.keyboard.isDown("a") and currentIteration < 11 then
			inTraining = true
			
			-- Espera-se que o usuário não tenha volume na voz caso chegue em notas que não alcança
			highestNote = "F4"
		
			-- Aqui ele canta com sucesso
			-- Passa-se para a nova fase do exercício
			if currTime - lastTime > 2 then
				currentIteration = currentIteration + 1
				lastTime = love.timer.getTime()
			end
			
		elseif love.keyboard.isDown("p") then
			state = 2
			inExample = true
			inTraining = false
			currentIteration = 1
		end
		
	-- Teste dos graves
	elseif state == 2 then
		-- Ouvir o exemplo
		if inExample and love.keyboard.isDown("o") then
			currentNote = lowNotes[currentIteration]
			note = Oscillator(currentNote)
			for i = 1, len*rate do
				sample = note() * amplitude
				soundData:setSample(i - 1, sample)
			end
	
			source = love.audio.newSource(soundData)
			love.audio.play(source)
		elseif love.keyboard.isDown("g") and currentIteration < 10 then
			inTraining = true
			
			-- Espera-se que o usuário não tenha volume na voz caso chegue em notas que não alcança
			lowestNote = "G2"
		
			-- Aqui ele canta com sucesso
			if currTime - lastTime > 2 then
				currentIteration = currentIteration + 1
				lastTime = currTime
			end
			
		elseif love.keyboard.isDown("f") then
			state = 3
			inExample = false
			inTraining = false
		end
		
	-- Tela de resultado
	elseif state == 3 and love.keyboard.isDown("r") then
		state = 0
		currentIteration = 1
	end
end

function love.draw()
	-- Tela inicial
	if state == 0 then 
		love.graphics.printf("Octavia", 300, 120, love.graphics.getWidth())
		love.graphics.printf("Pressione <enter> para começar", 250, 140, love.graphics.getWidth())
	
	-- Tela de teste de agudos
	elseif state == 1 then
		love.graphics.printf("Teste de agudos " .. currentIteration, 250, 120, love.graphics.getWidth())
		love.graphics.printf("Pressione <o> para ouvir o exercício", 250, 140, love.graphics.getWidth())
		love.graphics.printf("Pressione <a> para realizar o exercício, e depois novamente ao terminar", 250, 160, love.graphics.getWidth())
		love.graphics.printf("Pressione <p> para mudar o exercício", 250, 180, love.graphics.getWidth())
	
	-- Tela de teste de graves
	elseif state == 2 then
		love.graphics.printf("Teste de graves " .. currentIteration, 250, 120, love.graphics.getWidth())
		love.graphics.printf("Pressione <o> para ouvir o exercício", 250, 140, love.graphics.getWidth())
		love.graphics.printf("Pressione <g> para realizar o exercício, e depois novamente ao terminar", 250, 160, love.graphics.getWidth())
		love.graphics.printf("Pressione <f> para finalizar o exercício", 250, 180, love.graphics.getWidth())
	
	-- Tela de resultado
	elseif state == 3 then
		love.graphics.printf("Terminamos!", 250, 120, love.graphics.getWidth())
		love.graphics.printf("Seu alcance é de " .. lowestNote .. " a " .. highestNote .. "!", 250, 140, love.graphics.getWidth())
		love.graphics.printf("Pressione <r> para recomeçar", 250, 160, love.graphics.getWidth())
	end
end
