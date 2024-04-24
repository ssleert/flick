extractcss:
	lessc style.less style.css
	cleancss style.css -o style-min.css

buildapp: extractcss
	cargo build --release

runapp: extractcss
	cargo run
