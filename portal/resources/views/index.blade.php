@extends('layout')

@section('title', 'Index')
@section('body')
<div id="app">
    {{-- Loading --}}
    {{-- <div v-if="!loaded" class="spinner-border" role="status">
        <span class="sr-only">Loading...</span>
    </div> --}}

    <router-view :selection="selection"></router-view>
</div>
@endsection