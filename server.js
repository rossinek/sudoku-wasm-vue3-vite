const express = require('express')
const path = require('path')

const app = express()

app.use('/public', express.static(path.join(__dirname, './sudoku-wasm-vue3-vite')))
app.use(express.urlencoded({ extended: false }))
app.use(express.json())

app.get('*', (req, res, next) => {
	res.sendFile(path.resolve(__dirname, './sudoku-wasm-vue3-vite/index.html'))
})

const server = app.listen(process.env.PORT || 3000, () => {
	console.log(`server running on port ${server.address().port}`)
})
