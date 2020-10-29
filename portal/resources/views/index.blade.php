@extends('layout')

@section('title', 'Index')
@section('body')
<div id="app">
    <div v-if="!loaded" class="spinner-border" role="status">
        <span class="sr-only">Loading...</span>
    </div>
    <h1 v-if="loaded">@{{ title }}</h1>

    {{-- Extensions --}}
    <table class="table" id='extensions' v-if='loaded && extensions.length > 0'>
        <thead>
            <tr>
                <th scope="col">#</th>
                <th scope="col">Name</th>
                <th scope="col">Actions</th>
            </tr>
        </thead>
        <tbody>
            <tr v-for="extension in extensions">
                <th scope="row">@{{ extension.id }}</th>
                <td>@{{ extension.name }}</td>
                <td>
                    <a v-on:click.prevent="getSessions(extension.id)" href='#'>Sessions</a>
                </td>
            </tr>
        </tbody>
    </table>

    {{-- Sessions --}}
    <table class="table" id='sessions' v-if='loaded && sessions.length > 0'>
        <thead>
            <tr>
                <th scope="col">#</th>
                <th scope="col">Actions</th>
            </tr>
        </thead>
        <tbody>
            <tr v-for="session in sessions">
                <th scope="row">@{{ session.id }}</th>
                <td>
                    <a v-on:click.prevent="getGenerations(session.id)" href='#'>Generations</a>
                </td>
            </tr>
        </tbody>
    </table>

    {{-- Generations --}}
    <table class="table" id='generations' v-if='loaded && generations.length > 0'>
        <thead>
            <tr>
                <th scope="col">#</th>
                <th scope="col">Actions</th>
            </tr>
        </thead>
        <tbody>
            <tr v-for="generation in generations">
                <th scope="row">@{{ generation.id }}</th>
                <td>
                    <a v-on:click.prevent="" href='#'>Statistics</a>
                </td>
            </tr>
        </tbody>
    </table>

    <p class='mt-3' v-if="(sessions.length + extensions.length + generations.length) == 0">
        No item found.
    </p>
</div>
@endsection